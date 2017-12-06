// $Id: BS2Serial.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Serial.h
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

#ifndef BS2SERIAL_H
#define BS2SERIAL_H

#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/DEV_Connector.h"
#include "BS2TTY_IO.h"
#include "BS2Driver.h"
#include "BS2BlockHeader.h"

static const int SECS1_RECEIVE_BUFSIZE = 512;
static const int SECS1_SEND_BUFSIZE = 512;

//-----------------------------------------------------------------------------
enum { ENQ = 0x05, EOT = 0x04, NAK = 0x15, ACK = 0x06 };
enum
{
    SECS1_WAIT = 0, SECS1_WAIT_EOT, SECS1_RECEIVED_EOT, SECS1_SENDING_DATA,
    SECS1_WAIT_ACK, SECS1_RECEIVED_ACK, SECS1_ABORT, SECS1_ENQ_COLLISION,
    SECS1_RECEIVE,
};

//-----------------------------------------------------------------------------
// SECS Serial Driver
//-----------------------------------------------------------------------------
class BEE_Export BS2Serial: public BS2Driver
{
// constructor/destoructor
public:
    BS2Serial();
    virtual ~BS2Serial();
//
    virtual void dump() const;
    virtual int  initialize(DeviceParameter * parm, BS2Device * device);
    virtual int  open();
    virtual int  recv(BCHAR * buf, int bufSize);
    virtual int  send(BS2TransactionInfo * trinfo);
    virtual int  close();
    virtual int  recv_thread();
    virtual bool canSend() const;
    virtual BS2OStream * makeLinktestMessage();
    virtual int  enable();
    virtual int  disable();
    virtual int  disconnect_endpoint();

private:
    bool clearBuffer();
    int  protocol(BYTE * readbuf, int bytes_read);
    int  recv_control(BYTE * readbuf);
    int  checksum(BYTE * buf, int bytes) const;
    int  send(BCHAR * data, int dataSize);
    int  send_sub(BCHAR * data, int dataSize);
    int  sendControl(BCHAR cbyte);

    void T2(long t2) { m_T2.msec(t2); }
    long T2() const  { return (m_T2.msec()); }

// variables
private:
    BCHAR m_commName[16];
    ACE_TTY_IO::Serial_Params m_params;
    BS2TTY_IO         m_commdev;
    ACE_DEV_Connector m_con;
    ACE_OVERLAPPED m_wos;
    ACE_OVERLAPPED m_ros;

    ACE_Time_Value  m_T1;
    ACE_Time_Value  m_T2;
    int  m_RTY;
    int  m_retry;
    ACE_Thread_Mutex m_lock;
    SEMIStatus  m_sendStatus;
    SEMIStatus  m_recvStatus;
    BCHAR m_rbuf[SECS1_RECEIVE_BUFSIZE];
    BCHAR m_sbuf[SECS1_SEND_BUFSIZE];
    ACE_Auto_Event m_recvEvent;

    BCHAR * m_userbuf;
    int  m_userbufSize;
    BS2BlockHeader m_before;
};


#endif




