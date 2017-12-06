// $Id: BS2Socket.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Socket.h
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

#ifndef BS2SOCKET_H
#define BS2SOCKET_H

#include "ace/OS.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Synch.h"
#include "BS2Driver.h"
#include "BS2BlockHeader.h"

static const int HSMS_RECEIVE_BUFSIZE = 32000;

//-----------------------------------------------------------------------------
enum { HSMS_NOT_CONNECTED = 0, HSMS_NOT_SELECTED = 1, HSMS_SELECTED = 2 };
enum { HSMS_REQ_NONE = 0, HSMS_REQ_STOP };
enum { HSMS_MODE_ACTIVE = 0, HSMS_MODE_PASSIVE = 1 };

//-----------------------------------------------------------------------------
// SECS Socket Driver
//-----------------------------------------------------------------------------
class BEE_Export BS2Socket: public BS2Driver
{
	friend void  SocketReadThreadProc(void * parm);
	friend class BS2SessionTimer;

public: // constructor/destoructor
    BS2Socket();
    virtual ~BS2Socket();
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

    int  getStatus() const { return m_status.value(); }
    int  sendSelectReq();
    int  sendSelectRsp(int status, BS2BlockHeader * req);
    int  sendDeselectReq();
    int  sendDeselectRsp(int status, BS2BlockHeader * req);
    int  sendLinktestReq();
    int  sendLinktestRsp(BS2BlockHeader * req);
    int  sendRejectReq(int status, BS2BlockHeader * req);
    int  sendSeparateReq();
			
protected:
    virtual int protocol(BYTE * readbuf, int bytes_read);
    int		close_endpoint();
    int		send(BCHAR * data, int dataSize, bool lengthSending = false);
    int		finish();
	int		isSocketError(int sockfd);
    void T5(long t5) { m_T5.msec(t5); }
    long T5() const  { return (m_T5.msec()); }
    void T6(long t6) { m_T6.msec(t6); }
    long T6() const  { return (m_T6.msec()); }
    void T7(long t7) { m_T7.msec(t7); }
    long T7() const  { return (m_T7.msec()); }
    void T8(long t8) { m_T8.msec(t8); }
    long T8() const  { return (m_T8.msec()); }

// variables
protected:
    int m_mode;
    ACE_OVERLAPPED m_wos;
    ACE_OVERLAPPED m_ros;
    ACE_SOCK_Acceptor  m_peer_acceptor;
    ACE_SOCK_Connector m_con;
    ACE_SOCK_Stream    m_stream;
    ACE_INET_Addr      m_addr;

    ACE_Thread_Mutex m_lock;
    SEMIStatus  m_status;
    SEMIStatus  m_reqStatus;
    SEMIStatus  m_stopConnect;
    ACE_Auto_Event m_recvEvent;

    ACE_Time_Value  m_T5;
    ACE_Time_Value  m_T6;
    ACE_Time_Value  m_T7;
    ACE_Time_Value  m_T8;
    int m_t7id;
    bool m_gateway;
	//BCHAR m_rbuf[HSMS_RECEIVE_BUFSIZE];
};

#endif
