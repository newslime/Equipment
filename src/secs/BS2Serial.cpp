// $Id: BS2Serial.cpp,v 1.10 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Serial.cpp
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 1998-2004 BEE Co.,Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
//=============================================================================

#define BEE_BUILD_DLL

#include "ace/TTY_IO.h"
#include "ace/Thread_Manager.h"
#include "BS2Serial.h"
#include "BS2Stream.h"
#include "BS2Device.h"
#include "BS2SECSReceiver.h"
#include "BS2TransactionManager.h"

static int SerialReadThreadProc(void * parm);


//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Serial::BS2Serial(): BS2Driver()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Serial::BS2Serial");

    m_commName[0] = '\0';
    m_recvbuf = m_rbuf;
    m_recvbufSize = sizeof(m_rbuf);
    m_sendbuf = m_sbuf;
    m_sendbufSize = sizeof(m_sbuf);
    ACE_OS::memset(&m_ros, 0, sizeof(ACE_OVERLAPPED));
    ACE_OS::memset(&m_wos, 0, sizeof(ACE_OVERLAPPED));
    m_sendStatus = SECS1_WAIT;
    m_recvStatus = SECS1_WAIT;
    m_userbuf = NULL;
    m_userbufSize = 0;
    m_T2.msec(10000);                     // 10 second.
    m_RTY = 3;
    ACE_OS::memset((BCHAR *)&m_before, 0, sizeof(BS2BlockHeader));
}

//-----------------------------------------------------------------------------
BS2Serial::~BS2Serial()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Serial::~BS2Serial");
#if defined(ACE_WIN32)
    if (m_ros.hEvent != NULL)
        ::CloseHandle(m_ros.hEvent);
    if (m_wos.hEvent != NULL)
        ::CloseHandle(m_wos.hEvent);
#endif
}

//-----------------------------------------------------------------------------
// Initialize
//-----------------------------------------------------------------------------
int BS2Serial::initialize(DeviceParameter * config, BS2Device * device)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::initialize");

    int result = BS2Driver::initialize(config, device);
    if (result < 0)
        return result;

    //
    CommParameter * parm = (CommParameter *)config;
    _tcsncpy(m_commName, parm->m_port, sizeof(m_commName)-1);
    m_commName[sizeof(m_commName)-1] = '\0';
    m_params.baudrate = parm->m_baudrate;
    if (_tcscmp(parm->m_parity, _TX("none")) == 0 ||
        _tcscmp(parm->m_parity, _TX("NONE")) == 0)
    {
        m_params.parityenb = 0;
        m_params.paritymode = NULL;
    }
    else
    {
        m_params.parityenb = 1;
        m_params.paritymode = parm->m_parity;
    }
    m_params.databits = parm->m_databit;
    m_params.stopbits = parm->m_stopbit;
    m_params.readtimeoutmsec = parm->m_read_timeout;
    m_params.ctsenb = 0;
    m_params.rcvenb = 1;

    // save timer values
    m_T2.msec(parm->m_t2timeout);           // T2
    m_RTY = parm->m_retry;
    BS2SECSReceiver * comm_recv = (BS2SECSReceiver *)m_device->getReceiver();
    comm_recv->T4(parm->m_t4timeout);
#if defined(ACE_WIN32)
    // create I/O event used for overlapped read
    m_ros.hEvent = ::CreateEvent(NULL,    // no security
                                 TRUE,    // explicit reset req
                                 FALSE,   // initial event reset
                                 NULL);   // no name
    if (m_ros.hEvent == NULL)
    {
        TRACE_ERROR((_TX("Failed to create event for read! TTY Error !\n")));
        return -1;
    }

    // create I/O event used for overlapped read
    m_wos.hEvent = ::CreateEvent(NULL,    // no security
                                 TRUE,    // explicit reset req
                                 FALSE,   // initial event reset
                                 NULL);   // no name
    if (m_wos.hEvent == NULL)
    {
        TRACE_ERROR((_TX("Failed to create event for write! TTY Error !\n")));
        return -1;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------
int BS2Serial::open()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::open");

    if (m_open == true)
    {
        TRACE_ERROR((_TX("comm-port is opened %s\n"), m_commName));
        return -1;
    }

#if !defined(ACE_WIN32)
    if (m_con.connect(m_commdev, ACE_DEV_Addr(m_commName)) == -1)
#else
    if (m_con.connect (m_commdev,
               ACE_DEV_Addr(m_commName),
               0,
               ACE_Addr::sap_any,
               0,
               O_RDWR | FILE_FLAG_OVERLAPPED) == -1)
#endif
    {
        TRACE_ERROR((_TX("%s is not connected \n"), m_commName));
        return -1;
    }

    int ret = m_commdev.control(ACE_TTY_IO::SETPARAMS, &m_params);
    if (ret == -1)
    {
        TRACE_ERROR((_TX("%s control\n"), m_commName));
        return -1;
    }

    m_open = true;  // In Linux, before starting a thread, you have to set OPEN flag.
    ACE_Thread_Manager * _tm = ACE_Thread_Manager::instance();
    if (_tm->spawn((ACE_THR_FUNC)SerialReadThreadProc, (void *)this) == -1)
    {
        m_open = false;
        TRACE_ERROR((_TX("theread create for SerialReadThreadProc failed\n")));
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Close
//-----------------------------------------------------------------------------
int BS2Serial::close()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::close");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("comm-port is not opened %s\n"), m_commName));
        return -1;
    }
    m_open = false;
    m_commdev.close();
    return 0;
}

//-----------------------------------------------------------------------------
// Enable/Disable
//-----------------------------------------------------------------------------
int BS2Serial::enable()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::enable");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("comm-port is not opened %s\n"), m_commName));
        return -1;
    }
    m_disable = 0;
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Serial::disable()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::disable");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("comm-port is not opened %s\n"), m_commName));
        return -1;
    }
    m_disable = 1;
    return 0;
}

//-----------------------------------------------------------------------------
// Disconnect Endpoint
//-----------------------------------------------------------------------------
int BS2Serial::disconnect_endpoint()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::disconnect_endpoint");
    return 0;
}


//-----------------------------------------------------------------------------
// Send SECS Stream buffer
//-----------------------------------------------------------------------------
int BS2Serial::send(BS2TransactionInfo * trinfo)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::send");

    BS2IStream * istmbuf = NULL;
    BS2OStream * ostmbuf;
    BS2BlockHeader * header;
    BCHAR data[SECS1_BLOCK_SIZE+32];

    while (m_recvStatus == SECS1_RECEIVE)
    {
        ACE_Time_Value tv(0, 125000);
        ACE_OS::sleep(tv);
    }

    ostmbuf = trinfo->buffer();
    // make block header
    header = (BS2BlockHeader *)ostmbuf->ptop(); // header point <- stream buffer
    header->setDeviceId(m_deviceId);
    if (m_slave != true)
        header->setToHostBit();
    trinfo->blockCount(1);                     // initialize block number
    header->setBlockNum(trinfo->blockCount());
    header->setSourceNum(GET_SOURCE_NUMBER(trinfo->self()));
    header->setTransactionNum(GET_TRANSACTION_NUMBER(trinfo->self()));
    // restore block header in transaction
    trinfo->restoreHeader();

    istmbuf = ostmbuf->make();
    int length = istmbuf->size();
    int ssize = min(length, SECS1_BLOCK_SIZE);
    istmbuf->read(data, ssize);
    header = (BS2BlockHeader *)data;   // header point <- send buffer

    if (length > SECS1_BLOCK_SIZE)
    {   // Multi-block
        send(data, ssize);             // *** send first block ***
        length -= ssize;
        while (length > 0)
        {
            ssize = min((int)(SECS1_BLOCK_SIZE - sizeof(BS2BlockHeader)),
                        length);
            istmbuf->read(data + sizeof(BS2BlockHeader), ssize);

            if (trinfo->incBlockCount() != BEE_SUCCESS)
            {
                TRACE_ERROR((_TX("SECS-I block count over.\n")));
                return BEE_ERROR;
            }
            header->setBlockNum(trinfo->blockCount());
            if ((length - ssize) <= 0)
                header->setEndBit();
            // *** send a block ***
            send(data, ssize + sizeof(BS2BlockHeader));
            length -= ssize;
        }
    }
    else
    {   // Single block
        header->setEndBit();
        send(data, length);            // *** send a block ***
    }
    delete istmbuf;
    return BEE_SUCCESS;
}


//-----------------------------------------------------------------------------
// Send a block
//-----------------------------------------------------------------------------
int BS2Serial::send(BCHAR * data, int dataSize)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::send");

    int  result;
    BCHAR * sbuf;
    UINT check_sum;

    if (dataSize > 254)
    {
        TRACE_ERROR((_TX("Send data size over !\n")));
        return -1;
    }

    sbuf = m_sendbuf;
    *sbuf++ = dataSize;
    check_sum = 0;
    for (int i = 0; i < dataSize; i++)
    {
        check_sum += (BYTE)(*data);
        check_sum &= 0xFFFF;
        *sbuf++ = *data++;
    }
    *sbuf++ = (check_sum >> 8) & 0xFF;
    *sbuf++ = check_sum & 0xFF;
    m_sendSize = dataSize + 1 + 2;      // size + check_sum + data

    m_retry = 0;
    do
    {
        do
        {
            m_sendStatus = SECS1_WAIT_EOT;
            sendControl(ENQ);              // send ENQ
            ACE_Time_Value abs_time(ACE_OS::gettimeofday());
            abs_time += m_T2;
            result = m_recvEvent.wait(&abs_time);
            if (result >= 0)
            {
                if (m_sendStatus == SECS1_RECEIVED_EOT)
                    break;
                else
                {    // may be received SECS1_ABORT or SECS1_ENQ_COLLISION
                     TRACE_ERROR((_TX("%T not <EOT> received. abort ? \n")));
                }
            }
            else
            {
                TRACE_ERROR((_TX("%T <EOT> T2 timeout (%d)\n"), m_retry));
                m_retry++;
            }
        } while ((m_sendStatus != SECS1_RECEIVED_EOT) && (m_retry <= m_RTY));

        if ((m_retry > m_RTY) || (m_sendStatus == SECS1_ABORT))
        {
            result = BEE_ERROR;              // send error !
            m_sendStatus = SECS1_WAIT;
            return result;
        }

        // send data
        m_sendStatus = SECS1_SENDING_DATA;
        int retval = send_sub(m_sendbuf, m_sendSize);
        while (retval < 0)
        {
            ACE_Time_Value tv(0, 100);
            ACE_OS::sleep(tv);
            retval = send_sub(m_sendbuf, m_sendSize);
        }

        // wait ack
        m_sendStatus = SECS1_WAIT_ACK;
        ACE_Time_Value abs_time(ACE_OS::gettimeofday());
        abs_time += m_T2;
        result = m_recvEvent.wait(&abs_time);
        if (result >= 0)
        {
            if(m_sendStatus == SECS1_RECEIVED_ACK)
            {
                break;
            }
            else  // maybe SECS1_ABORT
            {
                 TRACE_ERROR((_TX("%T not <ACK> received. abort ? \n")));
                 break;
            }
        }
        else
        {
            TRACE_ERROR((_TX("%T <ACK> T2 timeout (%d)\n"), m_retry));
            m_retry++;
        }
    } while ((m_sendStatus != SECS1_RECEIVED_ACK) && (m_retry <= m_RTY));

    // next block
    if ((m_retry > m_RTY) || (m_sendStatus == SECS1_ABORT))
        result = BEE_ERROR;              // send error !
    else
        result = BEE_SUCCESS;

    m_sendStatus = SECS1_WAIT;
    return result;
}

//-----------------------------------------------------------------------------
// Send sub-routine
//-----------------------------------------------------------------------------
int BS2Serial::send_sub(BCHAR * data, int dataSize)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::send_sub");
    ssize_t bytes_write;

#if defined(ACE_WIN32)
    int     writeStat;
    ULONG   errFlags;
    ULONG   errCode;
    ULONG   bytesSent = 0;
    COMSTAT commStat;

    ACE_MT(ACE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1));

    writeStat = ::WriteFile(m_commdev.get_handle(), data, dataSize,
                            (ULONG *)&bytes_write, &m_wos);
    if (! writeStat)
    {
        if (::GetLastError() == ERROR_IO_PENDING)
        {
            while (! ::GetOverlappedResult(m_commdev.get_handle(),
                                   &m_wos, (ULONG *)&bytes_write, TRUE))
            {
                errCode = ::GetLastError();
                if (errCode == ERROR_IO_INCOMPLETE)
                {    // normal result if not finished
                     bytesSent += bytes_write;
                     continue;
                }
                else
                {
                    ::ClearCommError(m_commdev.get_handle(), &errFlags,
                                     &commStat);
                    break;
                }
            }
            bytesSent += bytes_write;

            if (bytesSent != (ULONG)dataSize)
            {
                TRACE_ERROR((_TX("Probable Write Timeout: Total of %ld bytes sent \n"),
                             bytesSent));
            }
            else
            {
               if (m_hexdump)
               {
                   ACE_HEX_DUMP((LM_DEBUG, data, dataSize));
                   ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
               }
            }
        }
    }
    else
    {
        if (m_hexdump)
        {
            ACE_HEX_DUMP((LM_DEBUG, data, dataSize));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
        }
    }
#else
    ACE_OVERLAPPED ovl;

    ACE_MT(ACE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1));

    bytes_write = m_commdev.send((void *)data, dataSize, &ovl);
    if (bytes_write != dataSize)
    {
        TRACE_ERROR((_TX("Illegal send data (%d:%d) !\n"), dataSize, bytes_write));
        return -1;
    }
    if (m_hexdump)
    {
        ACE_HEX_DUMP((LM_DEBUG, data, dataSize));
        ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
// Send control byte
//-----------------------------------------------------------------------------
int BS2Serial::sendControl(BCHAR cbyte)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::sendControl");

    int result;
    BCHAR cbuf[4];
    cbuf[0] = cbyte;
    result = send_sub(cbuf, 1);            // send control code
    while (result < 0)
    {
        ACE_Time_Value tv(0, 100);
        ACE_OS::sleep(tv);
        result = send_sub(cbuf, 1);        // send control code
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Receive
//-----------------------------------------------------------------------------
int BS2Serial::recv(BCHAR *, int)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::recv");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return BEE_ERROR;
}

//-----------------------------------------------------------------------------
// Can application send message ?
//-----------------------------------------------------------------------------
bool BS2Serial::canSend() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::canSend");

    return this->isEnable();
}

//-----------------------------------------------------------------------------
// Make Linktest Message
//-----------------------------------------------------------------------------
BS2OStream * BS2Serial::makeLinktestMessage()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::makeLinktestMessage");

    return NULL;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Serial::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_BEGIN_DUMP, this));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_commName = %s"), this->m_commName));
    this->m_commdev.dump();
    this->m_con.dump();
    BS2Driver::dump();
    ACE_DEBUG((LM_DEBUG, ACE_END_DUMP));
}

//-----------------------------------------------------------------------------
// clear receive buffer
//-----------------------------------------------------------------------------
bool BS2Serial::clearBuffer()
{
#if defined(ACE_WIN32)
    COMSTAT     commStat;
    ULONG       errFlags;

    ::ClearCommError(m_commdev.get_handle(), &errFlags, &commStat);
    ::PurgeComm(m_commdev.get_handle(), PURGE_TXABORT | PURGE_RXABORT |
                                        PURGE_TXCLEAR | PURGE_RXCLEAR);
#endif
    return true;
}

//-----------------------------------------------------------------------------
// calc. check-sum
//-----------------------------------------------------------------------------
int BS2Serial::checksum(BYTE * buf, int bytes) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::checksum");

    int sum = 0;
    for (int i = 0; i < bytes; i++)
    {
        sum += *buf++;
        sum &= 0xFFFF;
    }
    return sum;
}

//-----------------------------------------------------------------------------
//
// comm-port read thread procedure
//
//-----------------------------------------------------------------------------
static int SerialReadThreadProc(void * parm)
{
    BS2Serial * eqcomm = (BS2Serial *)parm;
    return eqcomm->recv_thread();
}

//-----------------------------------------------------------------------------
// Thread procedure
//-----------------------------------------------------------------------------
int BS2Serial::recv_thread()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::recv_thread");

    TRACE_DEBUG((_TX("start.\n")));

    int  bytes_read;
    int  result;
    BCHAR * readbuf = recvbuf();

    if (readbuf == NULL)
    {
        TRACE_ERROR((_TX("No buffer for serial read !\n")));
        return -1;
    }

#if defined(ACE_WIN32)
    int     bufsize = recvbufSize();
    ULONG   eventMask;
    COMSTAT commStat;
    ULONG   errFlags;

    if (! ::SetCommMask(m_commdev.get_handle(), EV_RXCHAR))
    {
        TRACE_ERROR((_TX("Failed to mask serial communication !\n")));
        return -1;
    }
#endif

    while (m_open)
    {
#if defined(ACE_WIN32)
        eventMask = 0;
        ::WaitCommEvent(m_commdev.get_handle(), &eventMask, NULL);
        if ((eventMask & EV_RXCHAR) != EV_RXCHAR)
            continue;

        ::ClearCommError(m_commdev.get_handle(), &errFlags, &commStat ) ;
        int readStatus = ::ReadFile(m_commdev.get_handle(), readbuf,
                                     bufsize, (ULONG *)&bytes_read, &m_ros);
        if (! readStatus)
        {
            if (::GetLastError() == ERROR_IO_PENDING)
            {
                while (! ::GetOverlappedResult(m_commdev.get_handle(),
                                               &m_ros, (ULONG *)&bytes_read,
                                               TRUE))
                {
                    ULONG errCode = ::GetLastError();
                    if (errCode == ERROR_IO_INCOMPLETE)
                        continue;

                    TRACE_ERROR((_TX("%s(0x%x) \n"), m_commName, errCode));

                    ::ClearCommError(m_commdev.get_handle(),
                                     &errFlags, &commStat);
                    bytes_read = 0;
                    break;
                }
            }
            else
            {
                bytes_read = 0;
            }
        }
        if (bytes_read >= 1)
        {
            if (m_hexdump)
            {
                ACE_HEX_DUMP((LM_DEBUG, readbuf, bytes_read));
                ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
            }
            //
            result = protocol((BYTE *)readbuf, bytes_read);
        }
#else
        clearBuffer();
        BYTE cbuf[2];
        int  bufpos;
        bytes_read = m_commdev.recv ((void *)cbuf, 1, &m_ros);
        if (bytes_read == -1)
        {
            TRACE_ERROR((_TX("%s receive error \n"), m_commName));
        }
        else if (bytes_read == 1)
        {
            if (cbuf[0] >= 10)
            {
                bufpos = 0;
                readbuf[bufpos++] = cbuf[0];
                do
                {
                    bytes_read = m_commdev.recv ((void *)cbuf, 1, &m_ros);
                    readbuf[bufpos++] = cbuf[0];
                } while (bytes_read == 1 && bufpos <= ((BYTE)readbuf[0] + 2));

                if (bytes_read == 1)
                {
                    bytes_read = (BYTE)readbuf[0] + 3;
                }
            }
            else
            {
                readbuf[0] = cbuf[0];
            }
        }
        else
        {
            TRACE_ERROR((_TX("receive data size error: %d \n"), bytes_read));
        }

        if (bytes_read >= 1)
        {
            if (m_hexdump)
            {
                ACE_HEX_DUMP((LM_DEBUG, readbuf, bytes_read));
                ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
            }
            result = protocol((BYTE *)readbuf, bytes_read);
        }
        else if (bytes_read == -1)
        {
            clearBuffer();
            TRACE_ERROR((_TX("%s receive error \n"), m_commName));
        }
#endif
    }

    m_device->disconnected();   // Notify disconnected to application

    return 0;
}

//-----------------------------------------------------------------------------
// SECS-I protocol procedure
//-----------------------------------------------------------------------------
int BS2Serial::protocol(BYTE * dptr, int dataq)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Serial::protocol");

    BYTE * readbuf = dptr;
    int  bytes_read = dataq;

    while (bytes_read > 0)
    {
        if (*readbuf == ENQ || *readbuf == EOT || *readbuf == ACK ||
            (*readbuf == NAK&&m_recvStatus != SECS1_RECEIVE))
        {
            recv_control(readbuf);
            bytes_read--;
            readbuf++;
        }
        else
            break;
    }

    if (bytes_read > 0)
    {
        // receive data
        if (m_recvStatus == SECS1_RECEIVE)
        {
            m_recvSize = (BYTE)*readbuf;
            if ((bytes_read == m_recvSize + 1 + 2) &&
                (m_recvSize >= (int)sizeof(BS2BlockHeader) &&
                 m_recvSize <= SECS1_BLOCK_SIZE))
            {
                int recvsum = ((((BYTE)*(readbuf + m_recvSize + 1)) << 8) +
                               (BYTE)*(readbuf + m_recvSize + 2));
                int calcsum = checksum(readbuf + 1, m_recvSize);
                if (calcsum == recvsum)
                {
                    BS2BlockHeader * bheader;
                    bheader = (BS2BlockHeader *)(m_recvbuf + 1);
                    if (m_before.isEqual(bheader))
                    {   // ignore:  sendControl(NAK);
                        m_recvStatus = SECS1_ABORT;
                    }
                    else
                    {
                        m_before = *bheader;   // save header data.

                        // *** notify upper layer ***
                        if (m_device->getReceiver()->dispatch(m_recvbuf + 1,
                                                              m_recvSize) !=
                            BEE_SUCCESS)
                        {
                            sendControl(NAK);
                            m_recvStatus = SECS1_ABORT;
                        }
                        else
                        {
                            sendControl(ACK);
                            m_recvStatus = SECS1_WAIT;
                        }
                    }
                }
                else
                {
                    TRACE_ERROR((_TX("check-sum error 0x%X:0x%X\n"),
                                 recvsum, calcsum));
                    sendControl(NAK);
                    m_recvStatus = SECS1_WAIT;
                }
            }
            else
            {
                TRACE_ERROR((_TX("received data size error %d:%d\n"),
                             m_recvSize, bytes_read));
                sendControl(NAK);
                m_recvStatus = SECS1_WAIT;
            }
            if (m_sendStatus == SECS1_ENQ_COLLISION)
                m_recvEvent.signal();
        }
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// SECS-I protocol procedure (case control code)
//-----------------------------------------------------------------------------
int BS2Serial::recv_control(BYTE * readbuf)
{
    if (*readbuf == ENQ)
    {   // send data size
        if (m_slave == true)
        {   /* host */
            sendControl(EOT);
            m_recvStatus = SECS1_RECEIVE;
            if (m_sendStatus == SECS1_WAIT_EOT)
            {
                m_sendStatus = SECS1_ENQ_COLLISION;
                // m_recvEvent.signal();
            }
        }
        else
        {
            if (m_sendStatus == SECS1_WAIT_EOT ||
                m_sendStatus == SECS1_WAIT_ACK)
            {
                ; // ignore ENQ from host
            }
            else if (m_recvStatus == SECS1_WAIT)
            {
                sendControl(EOT);
                m_recvStatus = SECS1_RECEIVE;
            }
            else
            {
                sendControl(NAK);
            }
        }
    }
    else if (*readbuf == EOT)
    {
        if (m_sendStatus == SECS1_WAIT_EOT)
        {
            m_sendStatus = SECS1_RECEIVED_EOT;
            m_recvEvent.signal();
        }
    }
    else if (*readbuf == ACK)
    {
        if (m_sendStatus == SECS1_WAIT_ACK)
        {
            m_sendStatus = SECS1_RECEIVED_ACK;
            m_recvEvent.signal();
        }
    }
    else if (*readbuf == NAK)
    {
        if (m_sendStatus == SECS1_WAIT_ACK || m_sendStatus == SECS1_WAIT_EOT)
        {
            // m_sendStatus = SECS1_ABORT;
            // m_recvEvent.signal();
        }
    }
    return BEE_SUCCESS;
}

