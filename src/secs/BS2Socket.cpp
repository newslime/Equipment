// $Id: BS2Socket.cpp,v 1.11 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Socket.cpp
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

#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "BS2Socket.h"
#include "BS2Stream.h"
#include "BS2Device.h"
#include "BS2Sender.h"
#include "BS2Receiver.h"
#include "BS2TransactionManager.h"
#include "BS2Message.h"

void SocketReadThreadProc(void * parm);
static const int SOCK_READBUF_SIZE = 64000;

//-----------------------------------------------------------------------------
/**
 *  @class   BS2SessionTimer
 *  @brief   Event handler for T7 Session timer queue timeout events.
 *  @note
 *  The <handle_timeout> hook method prints out the current
 * time, prints the time when this timer expired and deletes "this".
 */
class BS2SessionTimer : public BS2TimerHandler
{
public:
    BS2SessionTimer(const ACE_Time_Value& expiration_time, BS2Socket* socketPtr)
	: BS2TimerHandler(expiration_time), m_socket(socketPtr) 
	{ }
    
	~BS2SessionTimer(void) 
	{ }

    // Call back hook.
    virtual int handle_timeout(const ACE_Time_Value&, const void *) 
	{
		m_socket->close_endpoint();
		delete this;
		return 0;
    }
    // The handler was cancelled, so we must delete this.
    virtual int cancelled(void) { delete this; return 0; }

private:
    BS2Socket * m_socket;                // Store the manager of send message.
};

//-----------------------------------------------------------------------------
//
// comm-port read thread procedure
//
//-----------------------------------------------------------------------------
void SocketReadThreadProc(void* parm)
{
    BS2Socket* eqcomm = (BS2Socket *)parm;

	eqcomm->BS2Driver::m_bs2driver_loop = 1;

    while (eqcomm->m_reqStatus != HSMS_REQ_STOP && eqcomm->m_disable == 0)
    {
        int result  = eqcomm->recv_thread();
        if (result < 0)
        {
            break;
        }

        //ACE_OS::sleep(1);
    }

    eqcomm->m_reqStatus = HSMS_REQ_NONE;
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) terminate hsms receive thread. \n")));
    
	eqcomm->BS2Driver::m_bs2driver_loop = 0;
}

//-----------------------------------------------------------------------------
// Constructor/Destoructor
//-----------------------------------------------------------------------------
BS2Socket::BS2Socket() : BS2Driver()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Socket::BS2Socket");

    if ((m_recvbuf = (BCHAR *)malloc(SOCK_READBUF_SIZE)) != NULL)
        m_recvbufSize = SOCK_READBUF_SIZE;

    ACE_OS::memset(&m_ros, 0, sizeof(ACE_OVERLAPPED));
    ACE_OS::memset(&m_wos, 0, sizeof(ACE_OVERLAPPED));
    m_status	= HSMS_NOT_CONNECTED;
    m_reqStatus = HSMS_REQ_NONE;
    m_slave		= false;
    m_gateway	= false;
}

//-----------------------------------------------------------------------------
BS2Socket::~BS2Socket()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Socket::~BS2Socket");
#ifdef _MSC_VER
    if (m_recvbuf != NULL)
        free(m_recvbuf);
    if (m_ros.hEvent != NULL)
        ::CloseHandle(m_ros.hEvent);
    if (m_wos.hEvent != NULL)
        ::CloseHandle(m_wos.hEvent);
#endif
}

//-----------------------------------------------------------------------------
// Initialize
//-----------------------------------------------------------------------------
int BS2Socket::initialize(DeviceParameter * config, BS2Device * device)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::initialize");

    int result = BS2Driver::initialize(config, device);
    if (result < 0)
        return result;

    //
    SocketParameter * parm = (SocketParameter *)config;
    m_mode = parm->m_mode;
    if (parm->m_mode == HSMS_MODE_ACTIVE)
    {
        if (parm->m_hostname)
            m_addr.set((u_short)parm->m_port, parm->m_hostname);
        else
            m_addr.set((u_short)parm->m_port, "localhost");
    }
    else
    {
        m_addr.set((u_short)parm->m_port);
    }

    // save timer values
    m_T5.msec(parm->m_t5timeout);
    m_T6.msec(parm->m_t6timeout);
    m_T7.msec(parm->m_t7timeout);
    m_T8.msec(parm->m_t8timeout);

#ifdef _MSC_VER
    // create I/O event used for overlapped read
    m_ros.hEvent = ::CreateEvent(NULL,    // no security
                                 TRUE,    // explicit reset req
                                 FALSE,   // initial event reset
                                 NULL);   // no name
    if (m_ros.hEvent == NULL)
    {
        TRACE_ERROR((_TX("Failed to create event for read! SOCKET Error !\n")));
        return -1;
    }
    // create I/O event used for overlapped read
    m_wos.hEvent = ::CreateEvent(NULL,    // no security
                                 TRUE,    // explicit reset req
                                 FALSE,   // initial event reset
                                 NULL);   // no name
    if (m_wos.hEvent == NULL)
    {
        TRACE_ERROR((_TX("Failed to create event for write! SOCKET Error !\n")));
        return -1;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------
int BS2Socket::open()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::open");

    if (m_open == true)
    {
        TRACE_ERROR((_TX("socket is opened %s\n"), m_addr.get_host_addr()));
        return -1;
    }

    m_open = true;  // In Linux, before starting a thread, you have to set OPEN flag.
    ACE_Thread_Manager * _tm = ACE_Thread_Manager::instance();
    if (_tm->spawn((ACE_THR_FUNC)SocketReadThreadProc, (void *)this) == -1)
    {
        m_open = false;
        TRACE_ERROR((_TX("theread create for SocketReadThreadProc failed\n")));
        return -1;
    }
    m_disable = 0;
    m_stopConnect = 0;
    return 0;
}

//-----------------------------------------------------------------------------
// Close
//-----------------------------------------------------------------------------
int BS2Socket::close()
{
#if defined(__linux__)
	int sockfd_stream;
	int sockfd_peer;
#endif

	TRACE_FUNCTION(TRL_LOW, "BS2Socket::close");

    if (m_open == false)
    {
        //TRACE_ERROR((_TX("socket is not opened %s\n"), m_addr.get_host_addr()));
        return 0;
    }
    
	// Close new endpoint (listening endpoint stays open).
    /*if (m_stream.close() == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("close")));
    }*/

    m_reqStatus = HSMS_REQ_STOP;
	m_disable	= 0;
	
#if defined(__linux__)
	sockfd_stream = m_stream.get_handle();
    ACE_OS::shutdown(sockfd_stream, SHUT_RDWR);
#endif
	if (m_stream.close() == -1)
    	TRACE_ERROR((_TX("%p\n"), _TX("close")));

	if (m_mode == HSMS_MODE_PASSIVE)
    {
#if defined(__linux__)
    	sockfd_peer = m_peer_acceptor.get_handle();
    	ACE_OS::shutdown(sockfd_peer, SHUT_RDWR);
#endif
    	m_peer_acceptor.close();
    }
	
    m_stopConnect	= 1;
    m_open			= false;
	m_status		= HSMS_NOT_CONNECTED;

	while( BS2Driver::m_bs2driver_loop )
	{
#if defined(_WIN32)
		::Sleep(200);
#else
		usleep(200000);
#endif
	}

    return 0;
}

//-----------------------------------------------------------------------------
int BS2Socket::close_endpoint()
{
#if defined(__linux__)
	int sockfd_stream;
	int sockfd_peer;
#endif
	
	TRACE_FUNCTION(TRL_LOW, "BS2Socket::close_endpoint");

    if (m_open == false || m_reqStatus != HSMS_REQ_NONE)
    {
        TRACE_ERROR((_TX("socket is not opened %s\n"), m_addr.get_host_addr()));
        return 0;
    }
    
	// Close new endpoint (listening endpoint stays open).
    /*if (m_stream.close() == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("close")));
    }
    if (m_mode == HSMS_MODE_PASSIVE)
        m_peer_acceptor.close();*/
	
#if defined(__linux__)
	sockfd_stream = m_stream.get_handle();
    ACE_OS::shutdown(sockfd_stream, SHUT_RDWR);
#endif
    if (m_stream.close() == -1)
		TRACE_ERROR((_TX("%p\n"), _TX("close")));

	if (m_mode == HSMS_MODE_PASSIVE)
    {
#if defined(__linux__)
    	sockfd_peer = m_peer_acceptor.get_handle();
    	ACE_OS::shutdown(sockfd_peer, SHUT_RDWR);
#endif
    	m_peer_acceptor.close();
    }

    m_status = HSMS_NOT_CONNECTED;

    return 0;
}

//-----------------------------------------------------------------------------
// Enable/Disable
//-----------------------------------------------------------------------------
int BS2Socket::enable()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::enable");


    if (m_open == true)
    {
        TRACE_DEBUG((_TX("socket is opened %s\n"), m_addr.get_host_addr()));
        return 0;
    }

    m_status = HSMS_NOT_CONNECTED;
    m_reqStatus = HSMS_REQ_NONE;
    ACE_Thread_Manager * _tm = ACE_Thread_Manager::instance();
    if (_tm->spawn((ACE_THR_FUNC)SocketReadThreadProc, (void *)this) == -1)
    {
        TRACE_ERROR((_TX("theread create for SocketReadThreadProc failed\n")));
        return -1;
    }
    m_disable = 0;
    m_stopConnect = 0;
    m_open = true;

    return 0;
}

//-----------------------------------------------------------------------------
int BS2Socket::disable()
{
#if defined(__linux__)
	int sockfd_stream;
	int sockfd_peer;
#endif
	
	TRACE_FUNCTION(TRL_LOW, "BS2Socket::disable");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("socket is not opened %s\n"), m_addr.get_host_addr()));
        return 0;
    }
    
	m_reqStatus = HSMS_REQ_STOP;

    // Close new endpoint (listening endpoint stays open).
    /*if (m_stream.close() == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("close")));
    }
    if (m_mode == HSMS_MODE_PASSIVE)
        m_peer_acceptor.close();*/

#if defined(__linux__)
	sockfd_stream = m_stream.get_handle();
    ACE_OS::shutdown(sockfd_stream, SHUT_RDWR);
#endif
	if (m_stream.close() == -1)
    	TRACE_ERROR((_TX("%p\n"), _TX("close")));

	if (m_mode == HSMS_MODE_PASSIVE)
    {
#if defined(__linux__)
    	sockfd_peer = m_peer_acceptor.get_handle();
    	ACE_OS::shutdown(sockfd_peer, SHUT_RDWR);
#endif
    	m_peer_acceptor.close();
    }

    m_stopConnect	= 1;
    m_disable		= 1;
    m_open			= false;

    return 0;
}

//-----------------------------------------------------------------------------
// Close endpoint
//-----------------------------------------------------------------------------
int BS2Socket::disconnect_endpoint()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::disconnect_endpoint");

    if (this->getStatus() == HSMS_NOT_CONNECTED)
    {
        return 0;
    }

    if (m_open == false || m_reqStatus != HSMS_REQ_NONE)
    {
        TRACE_DEBUG((_TX("socket is not opened %s\n"), m_addr.get_host_addr()));
        return 0;
    }

    // Close new endpoint (listening endpoint stays open).
    if (m_stream.close_writer() == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("close_writer")));
    }

    return 0;
}

//-----------------------------------------------------------------------------
// Finish
//-----------------------------------------------------------------------------
int BS2Socket::finish()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::finish");

    this->sendSeparateReq();
    this->close_endpoint();
    return 0;
}

//-----------------------------------------------------------------------------
// Send SECS Stream buffer
//-----------------------------------------------------------------------------
int BS2Socket::send(BS2TransactionInfo * trinfo)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::send");

    int length;
    int retval;
	BS2OStream * ostmbuf;
    BS2IStream * istmbuf = NULL;
    BS2BlockHeader * header;

    if (m_open == false)
    {
        TRACE_ERROR((_TX("socket is not opened %s\n"), m_addr.get_host_addr()));
        return -1;
    }
    if (m_status == HSMS_NOT_CONNECTED)
    {
        TRACE_ERROR((_TX("socket is not connected %s\n"), m_addr.get_host_addr()));
        return -1;
    }

    ostmbuf = trinfo->buffer();
    // make block header
    header = (BS2BlockHeader *)ostmbuf->ptop();  // header point <- stream buffer
    if (trinfo->type() == TRANSACTION_SEND_SESSION)
    {
        header->setSourceNum(GET_SOURCE_NUMBER(trinfo->self()));
        header->setTransactionNum(GET_TRANSACTION_NUMBER(trinfo->self()));
        trinfo->timeout(m_T6);
    }
    else
    {
        header->setSessionId(m_deviceId);
        header->setPType(0);
        header->setSType(STYPE_DATA_MSG);
        header->setSourceNum(GET_SOURCE_NUMBER(trinfo->self()));
        header->setTransactionNum(GET_TRANSACTION_NUMBER(trinfo->self()));
    }
    // restore block header in transaction
    trinfo->restoreHeader();

    istmbuf = ostmbuf->make();
    length = istmbuf->size();

    retval = this->send((BCHAR *)header, length, true);   // *** send a block ***
    /*while (retval < 0)
    {
        ACE_Time_Value tv(0, 100);
        ACE_OS::sleep(tv);
        retval = this->send((BCHAR *)header, length, true);
    }*/
    delete istmbuf;
    
	return retval;
}


//-----------------------------------------------------------------------------
// Send a block
//-----------------------------------------------------------------------------
int BS2Socket::send(BCHAR * data, int dataSize, bool lengthSending)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::send");
    //ACE_MT(ACE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1));

    int		result;
	//BYTE	len[sizeof(long)];
	BYTE*	pcomdata;
    
	if (lengthSending)
    {
        /*BYTE len[sizeof(long)];
        len[0] = (dataSize >> 24) & 0xFF;
        len[1] = (dataSize >> 16) & 0xFF;
        len[2] = (dataSize >> 8) & 0xFF;
        len[3] = dataSize & 0xFF;
        
		result = m_stream.send(len, sizeof(long));
		if (result != sizeof(long))
			return BEE_ERROR;*/

		pcomdata	= new BYTE[sizeof(long)+dataSize];
		pcomdata[0] = (dataSize >> 24) & 0xFF;
        pcomdata[1] = (dataSize >> 16) & 0xFF;
        pcomdata[2] = (dataSize >> 8) & 0xFF;
        pcomdata[3] = dataSize & 0xFF;

		memcpy(pcomdata+sizeof(long), data, dataSize);
		
		//result = m_stream.send(pcomdata, sizeof(long)+dataSize);
		result = m_stream.send_n(pcomdata, sizeof(long)+dataSize, 0, &m_T8);
		
		delete pcomdata;

		if (result != sizeof(long)+dataSize)
			return BEE_ERROR;
    }
	else
	{
		//result = m_stream.send(data, dataSize);
		result = m_stream.send_n(data, dataSize, 0, &m_T8);

		if (result != dataSize)
			return BEE_ERROR;
	}

    if (m_hexdump)
    {
#if 0
        BYTE * bin = (BYTE *)data;
        ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("%T (S): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n"),
            *(bin + 0), *(bin + 1), *(bin + 2), *(bin + 3), *(bin + 4),
            *(bin + 5), *(bin + 6), *(bin + 7), *(bin + 8), *(bin + 9)));
        if (dataSize - 10 > 0)
        {
            ACE_HEX_DUMP((LM_DEBUG, data + 10, dataSize - 10));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
        }
#else
        ACE_HEX_DUMP((LM_DEBUG, data, dataSize));
#endif
    }
	
	/*result = m_stream.send(data, dataSize);
	if (result == dataSize)
        return BEE_SUCCESS;*/
    
	return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Receive
//-----------------------------------------------------------------------------
int BS2Socket::recv(BCHAR *, int)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::recv");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return BEE_ERROR;
}

//-----------------------------------------------------------------------------
// Can application send message ?
//-----------------------------------------------------------------------------
bool BS2Socket::canSend() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::canSend");

    if (! this->isEnable())
    {
        // TRACE_ERROR(("Driver disabled\n"));
        return false;
    }

    if (m_status == HSMS_SELECTED)
    {
        return true;
    }
    // TRACE_ERROR(("Status = 0%x\n", m_status));
    return false;
}

//-----------------------------------------------------------------------------
// Make Linktest Message
//-----------------------------------------------------------------------------
BS2OStream * BS2Socket::makeLinktestMessage()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::makeLinktestMessage");

    BS2Message h(0xFFFF, 0, 0, 0, STYPE_LINKTEST_REQ, 0);
    BS2OStream * io_buffer = new BS2OStream;
    *io_buffer << h;

    return io_buffer;
}

//-----------------------------------------------------------------------------
// Send Select Request
//-----------------------------------------------------------------------------
int BS2Socket::sendSelectReq()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendSelectReq");

    int result;
#ifdef HSMS_GS
    BS2Message h(m_deviceId, 0, 0, 0, STYPE_SELECT_REQ, 0);
#else
    BS2Message h(0xFFFF, 0, 0, 0, STYPE_SELECT_REQ, 0);
#endif
    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;

    //result = m_device->getSender()->send(out_buffer);   // send session block
	result = m_device->getSender()->dispatch(out_buffer);   // send session block
    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Select Response
//-----------------------------------------------------------------------------
int BS2Socket::sendSelectRsp(int status, BS2BlockHeader * req)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendSelectRsp");

    int result;
    
	UINT srcnum		= req->getSourceNum();
    UINT transnum	= req->getTransactionNum();
    
	BS2Message h(req->getSessionId(), 
				 0, 
				 0, 
				 status, 
				 STYPE_SELECT_RSP,
                 ((srcnum << 16) + transnum));

    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;
    //result = m_device->getSender()->send(out_buffer);    // send session block
	result = m_device->getSender()->dispatch(out_buffer);    // send session block
    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    
	return result;
}

//-----------------------------------------------------------------------------
// Send Deselect Request
//-----------------------------------------------------------------------------
int BS2Socket::sendDeselectReq()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendDeselectReq");

    int result;
#ifdef HSMS_GS
    BS2Message h(m_deviceId, 0, 0, 0, STYPE_DESELECT_REQ, 0);
#else
    BS2Message h(0xFFFF, 0, 0, 0, STYPE_DESELECT_REQ, 0);
#endif
    BS2OStream * o_buffer = new BS2OStream;
    *o_buffer << h;

    //result = m_device->getSender()->send(o_buffer);    // send session block
	result = m_device->getSender()->dispatch(o_buffer);    // send session block
    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Deselect Response
//-----------------------------------------------------------------------------
int BS2Socket::sendDeselectRsp(int status, BS2BlockHeader * req)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendDeselectRsp");

    int result;
    UINT srcnum = req->getSourceNum();
    UINT transnum = req->getTransactionNum();
    BS2Message h(req->getSessionId(), 0, 0, status, STYPE_DESELECT_RSP,
                 ((srcnum << 16) + transnum));
    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;
    //result = m_device->getSender()->send(out_buffer);    // send session block
	result = m_device->getSender()->dispatch(out_buffer);    // send session block
    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Linktest Request
//-----------------------------------------------------------------------------
int BS2Socket::sendLinktestReq()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendLinktestReq");

    int result;
    BS2Message h(0xFFFF, 0, 0, 0, STYPE_LINKTEST_REQ, 0);
    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;

    //result = m_device->getSender()->send(out_buffer);    // send session block
	result = m_device->getSender()->dispatch(out_buffer);    // send session block

    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Linktest Response
//-----------------------------------------------------------------------------
int BS2Socket::sendLinktestRsp(BS2BlockHeader * req)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendLinktestRsp");

    int result;
    UINT srcnum = req->getSourceNum();
    UINT transnum = req->getTransactionNum();
    BS2Message h(0xFFFF, 0, 0, 0, STYPE_LINKTEST_RSP,
                 ((srcnum << 16) + transnum));
    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;
    
	//result = m_device->getSender()->send(out_buffer);    // send session block
	result = m_device->getSender()->dispatch(out_buffer);    // send session block

    ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Reject Request
//-----------------------------------------------------------------------------
int BS2Socket::sendRejectReq(int status, BS2BlockHeader * req)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendRejectReq");

    int result;
    UINT srcnum = req->getSourceNum();
    UINT transnum = req->getTransactionNum();
    BYTE illegal_type = (req->getPType()) ? req->getPType() : req->getSType();
    BS2Message h(req->getSessionId(), 0, illegal_type, status,
                 STYPE_REJECT_REQ, ((srcnum << 16) + transnum));
    BS2OStream * out_buffer = new BS2OStream;
    *out_buffer << h;
    
	//result = m_device->getSender()->send(out_buffer);    // send session block
    result = m_device->getSender()->dispatch(out_buffer);    // send session block

	ACE_Time_Value tv(0, 100);
    ACE_OS::sleep(tv);
    return result;
}

//-----------------------------------------------------------------------------
// Send Separate Request
//-----------------------------------------------------------------------------
int BS2Socket::sendSeparateReq()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::sendSeparateReq");

    int result;
    BS2HSMSHeader h;
    h.m_length[0] = h.m_length[1] = h.m_length[2] = 0;
    h.m_length[3] = sizeof(BS2BlockHeader);
#ifdef HSMS_GS
    h.m_head.setSessionId(m_deviceId);
#else
    h.m_head.setSessionId(0xFFFF);
#endif
    h.m_head.setStreamNum(0);
    h.m_head.setFunctionNum(0);
    h.m_head.setPType(0);
    h.m_head.setSType(STYPE_SEPARATE_REQ);
    h.m_head.setSourceNum(m_sourceId);
    h.m_head.setTransactionNum(m_device->getTransactionManager()->getTransactionId());

    result = this->send((BCHAR *)&h, sizeof(BS2HSMSHeader));      // send session block
    while (result < 0)
    {
        ACE_Time_Value tv(0, 100);
        ACE_OS::sleep(tv);
        result = this->send((BCHAR *)&h, sizeof(BS2HSMSHeader));  // send session block
    }
    return BEE_SUCCESS;
}


//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Socket::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_BEGIN_DUMP, this));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nsock_addr = %s"), this->m_addr.get_host_addr()));
    BS2Driver::dump();
    ACE_DEBUG((LM_DEBUG, ACE_END_DUMP));
}

//-----------------------------------------------------------------------------
// HSMS protocol procedure
//-----------------------------------------------------------------------------
int BS2Socket::protocol(BYTE * dptr, int dataq)
{
    BS2TransactionManager*		trmgr	= m_device->getTransactionManager();
    BS2BlockHeader*				header	= (BS2BlockHeader *)dptr;
    BS2TransactionInfo*			trinfo;
    BS2SessionTimer*			t7timer;

    if (dataq >= (int)sizeof(BS2BlockHeader))
    {
        // receive data
        if (header->getPType() != 0)
        {
            sendRejectReq(HSMS_NOT_SUPPORT_PTYPE, header);
            return BEE_ERROR;
        }

        switch (header->getSType())
        {
            case STYPE_DATA_MSG:
                if (m_status == HSMS_SELECTED)
                {
                    header->setEndBit();       // compatibility of secs1
                    m_device->getReceiver()->dispatch((BCHAR *)dptr, dataq);
                }
                else
                {
                    sendRejectReq(HSMS_NOT_SELECTED_ENTITY, header);
                }
                break;
            case STYPE_SELECT_REQ:
                if (m_status == HSMS_NOT_SELECTED)
                {
                    m_status = HSMS_SELECTED;
                    
					m_device->getReceiver()->cancelTimer(m_t7id);    // Stop T7
										
					sendSelectRsp(HSMS_SELECT_OK, header);
                    ACE_DEBUG((LM_DEBUG, ACE_TEXT("start session.\n")));
                    //
                    //  Communication enable
                    m_device->connected();   // Notify connected to application (WAIT CRA)
                    //
                }
                else if (m_status == HSMS_SELECTED)
                {
                    sendSelectRsp(HSMS_SELECT_OK, header);
                    ACE_DEBUG((LM_DEBUG, ACE_TEXT("start session (colligion).\n")));
                }
                else
                {
#ifdef HSMS_GS
                    if (header->getSessionId() == m_deviceId)
                        sendSelectRsp(HSMS_SELECT_ACTIVE, header);
                    else
                        sendSelectRsp(HSMS_SELECT_CONNECTED, header);
#else
                    sendSelectRsp(HSMS_SELECT_ACTIVE, header);
#endif
                }
                break;
            case STYPE_SELECT_RSP:
                if (m_status == HSMS_NOT_SELECTED || m_status == HSMS_SELECTED)
                {
                    if (m_status == HSMS_NOT_SELECTED)
                    {
                        m_status = HSMS_SELECTED;
                        
						m_device->getReceiver()->cancelTimer(m_t7id);    // Stop T7
                        						
						ACE_DEBUG((LM_DEBUG, ACE_TEXT("start session.\n")));
                        //
                        //  Communication enable
                        m_device->connected();   // Notify connected to application (WAIT CRA)
                        //
                    }
                    trinfo = trmgr->buffer(header, TRANSACTION_SEND_SESSION);
                    if (trinfo != NULL)
                    {
                        m_device->getReceiver()->cancelTimer(trinfo->timer_id());
						trmgr->remove(trinfo);
                    }
                }
                break;
            case STYPE_DESELECT_REQ:
                if (m_status == HSMS_SELECTED)
                {
                    m_status = HSMS_NOT_SELECTED;
                    sendDeselectRsp(HSMS_DESELECT_OK, header);
                    // Start T7 timer
                    BS2SessionTimer* t7timer = new BS2SessionTimer(m_T7, this);
                    m_t7id = m_device->getReceiver()->addTimer(m_T7, (BS2TimerHandler *)t7timer);
                    					
					ACE_DEBUG((LM_DEBUG, ACE_TEXT("stop session.\n")));
                }
                else
                {
                    sendDeselectRsp(HSMS_DESELECT_NOTCONNECT, header);
                }
                break;
            case STYPE_DESELECT_RSP:
                m_status = HSMS_NOT_SELECTED;
                // Start T7 timer
                t7timer = new BS2SessionTimer(m_T7, this);
                m_t7id = m_device->getReceiver()->addTimer(m_T7, (BS2TimerHandler *)t7timer);
                				
				trinfo = trmgr->buffer(header, TRANSACTION_SEND_SESSION);
                if (trinfo != NULL)
                {
                    m_device->getReceiver()->cancelTimer(trinfo->timer_id());
                    trmgr->remove(trinfo);
                }
                ACE_DEBUG((LM_DEBUG, ACE_TEXT("stop session.\n")));
                break;
            case STYPE_LINKTEST_REQ:
                sendLinktestRsp(header);
                break;
            case STYPE_LINKTEST_RSP:
                trinfo = trmgr->buffer(header, TRANSACTION_SEND_SESSION);
                if (trinfo != NULL)
                {
                    m_device->getReceiver()->cancelTimer(trinfo->timer_id());
                    trmgr->remove(trinfo);
                }
                break;
            case STYPE_REJECT_REQ:
                break;
            case STYPE_SEPARATE_REQ:
                //this->close(); //can not close
                break;
            default:
                sendRejectReq(HSMS_NOT_SUPPORT_STYPE, header);
                return BEE_ERROR;
                break;
        }
    }
    else
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("received data size error %d\n"), dataq));
        return BEE_ERROR;
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Thread procedure
//-----------------------------------------------------------------------------
int BS2Socket::recv_thread()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Socket::recv_thread");
  
	int		r_bytes;
	size_t 	bytes_read;
    BYTE 	sizeBuf[sizeof(long)];
    ULONG 	rsize;
    int		result;
	int		mstimecount;
    BCHAR* 	readbuf = recvbuf();
    int   	bufsize = recvbufSize();

    if (readbuf == NULL)
    {
        TRACE_ERROR((_TX("No buffer for read !\n")));
        return -1;
    }
    if (m_open != true)
    {
        TRACE_ERROR((_TX("socket not opened !\n")));
        return -1;
    }

    if (m_mode == HSMS_MODE_PASSIVE)
    {
        if (m_peer_acceptor.open(m_addr) == -1)
        {
            TRACE_ERROR((_TX("%p\n"), _TX("open")));
            return -1;
        }

        if (m_peer_acceptor.get_local_addr(m_addr) == -1)
        {
            TRACE_ERROR((_TX("%p\n"), _TX("get_local_addr")));
            return -1;
        }

        // Activate server
        if (m_peer_acceptor.accept(m_stream) == -1)
        {
            TRACE_ERROR((_TX("%p\n"), _TX("accept")));
            m_peer_acceptor.close();
            return -1;
        }
        TRACE_DEBUG((_TX("Connected passive at port %d\n"),
                     m_addr.get_port_number()));
        m_status = HSMS_NOT_SELECTED;
 
        //leak
		if (! m_gateway)
        {   // Start T7 timer
            BS2SessionTimer* t7timer = new BS2SessionTimer(m_T7, this);
            m_t7id = m_device->getReceiver()->addTimer(m_T7, (BS2TimerHandler *)t7timer);
        }
    }
    else
    {
        int retval = m_con.connect(m_stream, m_addr);
        while (retval == -1)
        {
            // TRACE_ERROR((_TX("%p\n"), _TX("connect")));
            // return -1;
            ACE_OS::sleep(m_T5);
            if (m_stopConnect == 1)
            {
                TRACE_DEBUG((_TX("(%p) terminate hsms receive thread.\n")));
                return -1;
            }
            retval = m_con.connect(m_stream, m_addr);
        }
        TRACE_DEBUG((_TX("connected active at port %d.\n"), m_addr.get_port_number()));
        m_status = HSMS_NOT_SELECTED;

		//leak
        if (! m_gateway)
        {
            // Start T7 timer
            BS2SessionTimer * t7timer = new BS2SessionTimer(m_T7, this);
            m_t7id = m_device->getReceiver()->addTimer(m_T7, (BS2TimerHandler *)t7timer);
            						
			this->sendSelectReq();
        }
    }
		    
	mstimecount = 0;
	// receiving
	while(m_reqStatus != HSMS_REQ_STOP)
    {
        //r_bytes = m_stream.recv(sizeBuf, sizeof(long));
		r_bytes = m_stream.recv_n(sizeBuf, sizeof(long), 0, &m_T8);
        
		/*if (r_bytes == -1)
        {
            TRACE_ERROR((_TX("%p\n"), _TX("recv")));
            this->close_endpoint();   // called close
            //* m_device->getReceiver()->stopReceive();
            //* Close new endpoint (listening endpoint stays open).
            break;
        }*/
        //else if (r_bytes == 0)
		if (r_bytes == 0)
        {
            // Close new endpoint (listening endpoint stays open).
            this->close_endpoint();
            TRACE_DEBUG((_TX("(%P|%t) reached end of input, connection closed by endpoint\n")));
            break;
        }
        else if (r_bytes == (int)sizeof(long))
        {
			mstimecount = 0; //reset
			
			rsize = (sizeBuf[0] << 24)	
					+ 
					(sizeBuf[1] << 16)	
					+
                    (sizeBuf[2] << 8)	
					+
					sizeBuf[3];
            
			if ((int)rsize > bufsize)
            {
                TRACE_ERROR((_TX("receive data overflow (%d)\n"), rsize));
            }
            else
            {
                bytes_read = m_stream.recv_n(readbuf, rsize, 0, &m_T8);
                
				if (bytes_read != rsize)
                {
                    TRACE_ERROR((_TX("not equal received data size (%d:%d)\n"),
                                 bytes_read, rsize));
                    this->close_endpoint();   // called close
                    break;
                }
                else
                {
                    if (m_hexdump)
                    {
#if 0
                        BYTE * bin = (BYTE *)readbuf;
                        ACE_DEBUG((LM_DEBUG,
                            ACE_TEXT("%T (S): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n"),
                            *(bin + 0), *(bin + 1), *(bin + 2), *(bin + 3), *(bin + 4),
                            *(bin + 5), *(bin + 6), *(bin + 7), *(bin + 8), *(bin + 9)));
                        if (bytes_read - 10 > 0)
                        {
                            ACE_HEX_DUMP((LM_DEBUG, readbuf + 10, bytes_read - 10));
                            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" \n")));
                        }
#else
                        ACE_HEX_DUMP((LM_DEBUG, readbuf, bytes_read));
#endif
                    }
                    //
                    
					result = this->protocol((BYTE *)readbuf, bytes_read);
                }
            }
        }
        else
        {
			mstimecount += m_T8.msec();
			
			if( mstimecount >= 30*1000 ) //30 second check
			{
				mstimecount = 0; //reset
				
				if( isSocketError((int)m_stream.get_handle()) )
				{
					this->close_endpoint();
					TRACE_DEBUG((_TX("(%P|%t) reached end of input, connection closed by endpoint\n")));
					break;
				}
			}
			
			//TRACE_ERROR((_TX("not received message size (%d)\n"), r_bytes));
        }
    }

    //m_device->disconnected();   // Notify disconnected to application

    return 0;
}

int BS2Socket::isSocketError(int sockfd)
{
	int				ret_sel;
	int				result		= false;
	unsigned long	read_bytes	= 0;
	struct	timeval tv;
	fd_set			fd;
	
	tv.tv_sec	= 3;
	tv.tv_usec	= 0;

	FD_ZERO(&fd);
	FD_SET(sockfd, &fd);
	
	ret_sel = select(sockfd+1, &fd, 0, 0, &tv);

	if( ret_sel >= 1 )
	{
		//check data number in buffer
#if defined(_WIN32)
		ioctlsocket(sockfd, FIONREAD, &read_bytes);
#else
		ioctl(sockfd, FIONREAD, &read_bytes);
#endif

		if( read_bytes <= 0 )
			result = true;
	}

	return result;
}
