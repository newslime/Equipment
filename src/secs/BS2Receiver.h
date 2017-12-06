// $Id: BS2Receiver.h,v 1.8 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Receiver.h
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

#ifndef BS2RECEIVER_H
#define BS2RECEIVER_H

#include "beesecs.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Asynch_IO_Impl.h"
#include "ace/Asynch_IO.h"
#include "BS2TransactionManager.h"

// Forward declaration
class BS2Driver;
class BS2Device;
class BS2Message;
class BS2Sender;
class BS2Receiver;

typedef list<BS2MessageInfo *, allocator<BS2MessageInfo *> > BS2EventList;

//------------------------------------------------------------------------------
//  Event Handler of Timer
//------------------------------------------------------------------------------
/**
 *  @class    BS2TimerHandler
 *  @brief    Event handler for the timer queue timeout events.
 *  @note
 *  The <handle_timeout> hook method prints out the current
 * time, prints the time when this timer expired and deletes "this".
 */
class BEE_Export BS2TimerHandler : public ACE_Handler
{
public:
    BS2TimerHandler(const ACE_Time_Value& expiration_time)
            : ACE_Handler(), m_expires(expiration_time), m_id(0) {}
    virtual ~BS2TimerHandler(void) { }

    // Store an "id" for the Handler, which is only use to print better
    // messages.
    void set_id(int id) { this->m_id = id; }

    // Call back hook.
    virtual void handle_time_out(const ACE_Time_Value&, const void *) {
            return ;
        }
    // The handler was cancelled, so we must delete this.
    virtual int cancelled(void) { delete this; return 0; }

protected:
    // Store the expected time of expiration, it is used to print a nice
    // message saying how much delay was at the actual expiration time.
    ACE_Time_Value m_expires;

    // Store an "id" for the Handler, which is only use to print better
    // messages.
    int m_id;
};

//------------------------------------------------------------------------------
/**
 *  @class    BS2TransactionTimer
 *  @brief    Event handler for the timer queue timeout events.
 *  @note
 *  The <handle_timeout> hook method prints out the current
 * time, prints the time when this timer expired and deletes "this".
 */
class BS2TransactionTimer : public BS2TimerHandler
{
public:
    BS2TransactionTimer(const ACE_Time_Value& expiration_time,
                        BS2TransactionInfo * trinfo, BS2Device * device,
                        int retry = 0)
        : BS2TimerHandler(expiration_time), m_trinfo(trinfo),
          m_device(device), m_retry(retry), m_current(0) { }
    virtual ~BS2TransactionTimer(void) { }

    // Call back hook.
    virtual void handle_time_out(const ACE_Time_Value &current_time,
                                 const void * arg);
    // The handler was cancelled, so we must delete this.
    virtual int cancelled(void) { delete this; return 0; }

private:
    BS2TransactionInfo * m_trinfo;       // Store the information of transact.
    BS2Device * m_device;                // Store the manager of send message.
    int m_retry;
    int m_current;
};

//------------------------------------------------------------------------------
/**
 *  @class    BS2ReceiveTimer
 *  @brief    Event handler for the timer queue timeout events.
 *  @note
 *  The <handle_timeout> hook method prints out the current
 * time, prints the time when this timer expired and deletes "this".
 */
class BEE_Export BS2ReceiveTimer : public BS2TimerHandler
{
public:
    BS2ReceiveTimer(const ACE_Time_Value& expiration_time,
                    BS2Receiver * receiver)
        : BS2TimerHandler(expiration_time), m_receiver(receiver) { }
    virtual ~BS2ReceiveTimer(void) { }

    // Call back hook.
    virtual void handle_time_out(const ACE_Time_Value &current_time,
                                 const void * arg);
    // The handler was cancelled, so we must delete this.
    virtual int cancelled(void) { delete this; return 0; }

private:
    BS2Receiver * m_receiver;
};


//------------------------------------------------------------------------------
//
// SECS Message receiver
//
//------------------------------------------------------------------------------
class BEE_Export BS2Receiver: public MT_Task
{

friend class BS2Sender;
friend class BS2TransactionTimer;
friend class BS2ReceiveTimer;

public:
    /*BS2Receiver(BS2Device * device) : 
	m_device(device), 
	m_requestReceive(0),
	m_abort(false) 
	{}*/
	typedef std::map<int, BS2TimerHandler*> SESSION_TIMERMAP;

	BS2Receiver(BS2Device* device);
	~BS2Receiver();

    virtual int open(void * = 0);
    virtual int close(u_long = 0);

    // enqueue the message
    virtual int parse(BCHAR * data, int size);
	virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
    virtual int dispatch(BCHAR * data, int size);
	virtual int svc(void);              // Receive data from message queue.
        
    //int receive(BS2MessageInfo& rinfo);
	int	receive(BS2MessageInfo* ret, bool needtimeout);
    int receiveWithLimit(BS2MessageInfo* rinfo, ACE_Time_Value *tv = 0);
	int sense(BS2MessageInfo* rinfo);
    int stopReceive();

    // Add a new timer to expire in <seconds> more.
    int  addTimer(const ACE_Time_Value& interval, BS2TransactionInfo * trinfo);
    int  addTimer(const ACE_Time_Value& interval, BS2TimerHandler * tmh);
    int  cancelTimer(int timer_id);  // Cancel timer <id>.
    int  listTimer();                // List the current scheduled timers.
    	
	void	AddRecvTimer(int id, BS2TimerHandler* t7timer);
	void	RemoveRecvTimerbyId(int id);
	void	CancelRecvTimerAll();
	
	int notifyEventInfo(BS2MessageInfo* evtinfo);
	int getEventInfo(BS2MessageInfo*& retmsg);
    int putEventInfo(BS2MessageInfo*  evtinfo);
	
	volatile int m_bs2receiver_play;
	volatile int m_bs2receiver_loop;

protected:
    int	parseStreamNine(BCHAR* data, int size);
	int	Dispatch_Block(ACE_Message_Block* mb);

//protected:
    BS2Device*			m_device;
	ACE_Time_Value		m_timeout;
    ACE_Auto_Event		m_recvEvent;
    BS2EventList		m_events;
    ACE_Thread_Mutex	m_lock;          // lock for message list
    MUTEX_Int			m_requestReceive;
    MUTEX_Bool			m_abort;

	SESSION_TIMERMAP	m_SessionTimerMap;
};


#endif
