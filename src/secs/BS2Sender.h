// $Id: BS2Sender.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Sender.h
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

#ifndef BS2SENDER_H
#define BS2SENDER_H

#include "beesecs.h"
#include "BS2TransactionManager.h"

class BS2Driver;
class BS2Device;
class BS2Message;
class BS2Receiver;
class BS2OStream;
class BS2MsgHeader;
class BS2MessageInfo;

//------------------------------------------------------------------------------
// SECS Message sender
//------------------------------------------------------------------------------
//
class BEE_Export BS2Sender: public MT_Task
{
    friend class BS2Socket;
    friend class BS2Serial;

public:
    /*BS2Sender(BS2Device * device)
	: m_T3(0), m_T6(0), m_timeout(0), m_testime(0), m_device(device) 
	{}*/
	BS2Sender(BS2Device * device);
	~BS2Sender();

    virtual int open(void * = 0);
    virtual int close(u_long = 0);

    virtual int parse(BCHAR * data, int size);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
	virtual int send(BS2Message* msg, void * ptr = NULL);
	virtual int dispatch(BS2OStream * buff, void * ptr = NULL);
    virtual int svc(void);
    
    void T3(long t3) { m_T3.msec(t3); }
    void T6(long t6) { m_T6.msec(t6); }
    long T3() const  { return (m_T3.msec()); }
    long T6() const  { return (m_T6.msec()); }
    void setLinktestTime(long t) { m_testime.msec(t); }
    long getLinktestTime() const { return (m_testime.msec()); }

	volatile int m_bs2sender_play;
	volatile int m_bs2sender_loop;

protected:
    int	Dispatch_Block(ACE_Message_Block* mb);
		
private:
    ACE_Time_Value  m_T3;
    ACE_Time_Value  m_T6;
    ACE_Time_Value  m_timeout;
    ACE_Time_Value  m_testime;
    MT_Queue        m_waitq;
    BS2Device *     m_device;
};


#endif
