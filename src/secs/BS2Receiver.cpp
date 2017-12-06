// $Id: BS2Receiver.cpp,v 1.11 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Receiver.cpp
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

#include "beesecs.h"
#include "BS2Device.h"
#include "BS2Receiver.h"
#include "BS2Sender.h"
#include "BS2MessageDictionary.h"
#include "BS2Message.h"
#include "BS2Stream.h"
#include "BS2BlockHeader.h"
#include "BS2ItemHeader.h"
#include "BS2Driver.h"
#include "BS2MessageInfo.h"
#include "BS2ErrorMessage.h"

//-----------------------------------------------------------------------------
// Transaction time out.
//-----------------------------------------------------------------------------
void BS2TransactionTimer::handle_time_out(const ACE_Time_Value&,
                                          const void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionTimer::handle_time_out");
    BS2MessageInfo * msgcont = NULL;
    BS2MessageInfo * evtinfo = m_trinfo->eventHeader();

    BS2OStream * ostmbuf = m_trinfo->buffer();
    BCHAR * hptr = ostmbuf->ptop();
    switch (m_trinfo->type())
    {
        case TRANSACTION_SEND_PRIMARY:
            if (evtinfo != NULL)
            {
                msgcont = new BS2MessageInfo;
                evtinfo->moveto(msgcont);   // Move message
                msgcont->setResult(BS2RET_TIMEOUT);
                //m_device->getReceiver()->notifyEventInfo(msgcont);
				m_device->getReceiver()->putEventInfo(msgcont);
            }
            break;
        case TRANSACTION_RECV_PRIMARY:
            break;
        case TRANSACTION_RECV_SECONDARY:
            if (evtinfo != NULL)
            {
                msgcont = new BS2MessageInfo;
                evtinfo->moveto(msgcont);    // Move message
                evtinfo->setResult(BS2RET_TIMEOUT);
                //m_device->getReceiver()->notifyEventInfo(msgcont);
				m_device->getReceiver()->putEventInfo(msgcont);
            }
            break;
        case TRANSACTION_SEND_SESSION:
            break;
        default:
            TRACE_ERROR((_TX("Unknown transaction type (%d).\n"), m_trinfo->type()));
            hptr = NULL;
            break;
    }

    // Logging error information.
    TRACE_DEBUG((_TX("Transaction(0x%X) timeout.\n"), m_trinfo->self()));

    // Send S9F9(Transaction Timer timeout)
    if (hptr != NULL)
    {
        if (m_device->isEquipment() &&
            (m_trinfo->type() == TRANSACTION_RECV_PRIMARY))
        {
            BS2BlockHeader * header = (BS2BlockHeader *)hptr;
            BS2S9F9Message * s9f9 = new BS2S9F9Message(*header);
            m_device->getSender()->send(s9f9);
            delete s9f9;
        }
    }

    if (m_trinfo->getTransactionManager())
    {   // delete transaction.
        m_trinfo->getTransactionManager()->remove(m_trinfo);
    }
    
	delete this;
    //return ;
}

//-----------------------------------------------------------------------------
// Receive time out.
//-----------------------------------------------------------------------------
void BS2ReceiveTimer::handle_time_out(const ACE_Time_Value&, const void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ReceiveTimer::handle_time_out");

    m_receiver->stopReceive();
    delete this;
    //return ;
}

/////////////////////
BS2Receiver::BS2Receiver(BS2Device* device)
{
	m_requestReceive	= 0;
	m_bs2receiver_play	= 0;
	m_bs2receiver_loop	= 0;
	m_device			= device;
	m_abort				= false;
}

BS2Receiver::~BS2Receiver()
{
	m_bs2receiver_play = 0;
	while( m_bs2receiver_loop )
	{
#if defined(_WIN32)
		::Sleep(200);
#else
		usleep(200000);
#endif
	}

	//clear list only
	//remaining BS2MessageInfo will delete by BS2TransactionManager::removeAll
	this->m_events.clear();

	CancelRecvTimerAll();
}

//-----------------------------------------------------------------------------
//
// BS2Receiver
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Spawn off a new thread.
//-----------------------------------------------------------------------------
int BS2Receiver::open(void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::open");

	/*m_bs2receiver_play = 1;

    if (this->activate(THR_NEW_LWP | THR_DETACHED) == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("spawn")));
        return -1;
    }*/

    return 0;
}

//-----------------------------------------------------------------------------
int BS2Receiver::close(u_long exit_status)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::close");
    
	/*ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%t) thread is exiting with status %d in module %s\n"),
        exit_status, "BS2Receiver"));*/

    return 0;
}

//-----------------------------------------------------------------------------
// Simply enqueue the Message_Block into the end of the queue.
//-----------------------------------------------------------------------------
int BS2Receiver::put(ACE_Message_Block *mb, ACE_Time_Value *tv)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::put");
    return this->putq(mb, tv);
}

//-----------------------------------------------------------------------------
// Schedule a new timer.
//-----------------------------------------------------------------------------
int BS2Receiver::addTimer(const ACE_Time_Value& interval, BS2TransactionInfo * trinfo)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::addTimer");

    BS2TransactionTimer* trans_h = new BS2TransactionTimer(interval, trinfo, m_device);

    int id = ACE_Proactor::instance()->schedule_timer(*trans_h, 0, interval);
    if (id == -1)
    {
        TRACE_ERROR((_TX("schedule failed")));
        return BEE_ERROR;
    }
    // We store the id into the handler, this is only used to produce
    // nicer messages.
    // @@ Should this be something that a user can extract?
    trans_h->set_id(id);
    trinfo->timer_id(id);

    // TRACE_DEBUG((_TX("Start timer(%d) : timer id = %d, transaction = 0x%x\n"),
    //              interval.msec(), id, trinfo->self()));

	this->AddRecvTimer(id, trans_h);

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Schedule a new timer.
//-----------------------------------------------------------------------------
int BS2Receiver::addTimer(const ACE_Time_Value& interval, BS2TimerHandler * tmh)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::addTimer");

    int id = ACE_Proactor::instance()->schedule_timer(*tmh, 0, interval);
    if (id == -1)
    {
        TRACE_ERROR((_TX("schedule failed")));
        return BEE_ERROR;
    }
    // We store the id into the handler, this is only used to produce
    // nicer messages.
    tmh->set_id(id);

    // TRACE_DEBUG((_TX("Start timer(%d) : scheduling timer id = %d\n"),
    //              interval.msec(), id));

	this->AddRecvTimer(id, tmh);

    return id;
}

//-----------------------------------------------------------------------------
// Cancel a timer.
//-----------------------------------------------------------------------------
int BS2Receiver::cancelTimer(int timer_id)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::cancelTimer");

    if (timer_id < 0)
    {
        return BEE_ERROR;
    }
    int result = ACE_Proactor::instance()->cancel_timer(timer_id);

	this->RemoveRecvTimerbyId(timer_id);

    // TRACE_DEBUG((_TX("scheduling timer id = %d\n"), timer_id));
    return result;
}

//-----------------------------------------------------------------------------
// Lists the timers in the queue.  Ignores the argument.
//-----------------------------------------------------------------------------
int BS2Receiver::listTimer()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::listTimer");

    // Dump the timer queue contents.
    this->dump();

    return 0;
}

/////
void BS2Receiver::AddRecvTimer(int id, BS2TimerHandler* t7timer)
{
	m_SessionTimerMap.insert( std::map<int, BS2TimerHandler*>::value_type(id, t7timer) );
}

void BS2Receiver::RemoveRecvTimerbyId(int id)
{
	BS2TimerHandler*			t7timer;
	SESSION_TIMERMAP::iterator	mit;

	mit		= m_SessionTimerMap.find(id);
	t7timer = mit->second;
	
	if( t7timer )
		delete t7timer;

	m_SessionTimerMap.erase(mit);
}

void BS2Receiver::CancelRecvTimerAll()
{
	int							t_id;
	unsigned int				i;
	unsigned int				size;
	//BS2TimerHandler*			t7timer;
	SESSION_TIMERMAP::iterator	mit;
	
	mit		= m_SessionTimerMap.begin();
	size	= m_SessionTimerMap.size();
	
	for(i=0; i<size; i++)
	{
		t_id = mit->first;
		
		//this's deleted t7timer, by slime 
		m_device->getReceiver()->cancelTimer(t_id);    // Stop T7

		//mark by slime
		/*t7timer = mit->second;
		if( t7timer )
			delete t7timer;

		m_SessionTimerMap.erase(mit);*/
		
		mit = m_SessionTimerMap.begin();
	}
}
/////
//-----------------------------------------------------------------------------
// Receive message.
//-----------------------------------------------------------------------------
int BS2Receiver::receive(BS2MessageInfo* ret, bool needtimeout)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::receive");

	int					result;
	ACE_Time_Value		delaytm; //0.1 sec
	BS2MessageInfo*		rinfo = NULL;
	    
	ret->setMessage(NULL);
    ret->setResult(BS2RET_NORMAL);
    ret->setTransactionID(0);
    
	if (m_requestReceive == 1)
        return BEE_ERROR;
    
	m_requestReceive = 1;
    m_recvEvent.reset();
    m_abort = false;
    
    result = this->getEventInfo(rinfo);
    
	while( result < 0 )
    {   // Mutex locked
        if (m_abort.value())
        {   // stop to receive
            break;
        }
        
		result = this->getEventInfo(rinfo);
    }

    //if (result == 0)
	if( rinfo == NULL )
    {   
		if( !needtimeout )
		{
			if (m_recvEvent.wait() == -1)
			{
				TRACE_ERROR((_TX("SECS stream buffer receiving error.\n")));
			}
			else
			{
				result = this->getEventInfo(rinfo);
			}
		}
	}
			
    // received message
    //if ((! m_abort.value()) && (rinfo != NULL))
	if ( rinfo != NULL && !m_abort.value() )
    {
		rinfo->moveto(ret);
        		
		if (rinfo->canDelete())
        {
            delete rinfo;
        }
    }
   
	m_requestReceive = 0;                // unlock request

	return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Receive message with limit time.
//-----------------------------------------------------------------------------
int BS2Receiver::receiveWithLimit(BS2MessageInfo* ret, ACE_Time_Value *tv)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::receive");

    ret->setMessage(NULL);
    ret->setResult(BS2RET_NORMAL);
    ret->setTransactionID(0);
    
	if (m_requestReceive == 1)
        return BEE_ERROR;
    
	if (tv != NULL)
    {
        BS2ReceiveTimer * timer = new BS2ReceiveTimer(*tv, this);
        this->addTimer(*tv, timer);
    }

    return this->receive(ret, false);
}

//-----------------------------------------------------------------------------
// Sense received message.
//-----------------------------------------------------------------------------
int BS2Receiver::sense(BS2MessageInfo* ret)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::sense");

    int				result;
    BS2MessageInfo*	rinfo = NULL;

    ret->setMessage(NULL);
    ret->setResult(BS2RET_NORMAL);
    ret->setTransactionID(0);
    if (m_requestReceive == 1)
    {
        rinfo = NULL;
        result = BEE_ERROR;
    }
    else
    {
        m_requestReceive = 1;
        
		if (this->getEventInfo(rinfo) > 0)
        {   // received message
            rinfo->moveto(ret);
            
			if (rinfo->canDelete())
            {
                delete rinfo;
            
			}
            result = BEE_SUCCESS;
        }
        else
        {
            result = BEE_ERROR;
        }
        
		m_requestReceive = 0;                // unlock request
    }
    
	return result;
}

////
int BS2Receiver::notifyEventInfo(BS2MessageInfo* evtinfo) 
{
	putEventInfo(evtinfo);
	return m_recvEvent.signal();
}

//-----------------------------------------------------------------------------
// Get received message in queue.
//-----------------------------------------------------------------------------
int BS2Receiver::getEventInfo(BS2MessageInfo*& retmsg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::getEventInfo");
	
	ACE_MT(ACE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1));

	int size;
   
	size = this->m_events.size();

	if (size > 0)
    {
        //result = this->m_events.size();
		retmsg = (BS2MessageInfo*)this->m_events.front();

		this->m_events.pop_front();

		return BEE_SUCCESS;
    }

	return BEE_ERROR;
}

//-----------------------------------------------------------------------------
// Put received message in queue.
//-----------------------------------------------------------------------------
int BS2Receiver::putEventInfo(BS2MessageInfo* evtinfo)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::putEventInfo");
    
	ACE_MT(ACE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1));

    this->m_events.push_back(evtinfo);

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// End wait receiving.
//-----------------------------------------------------------------------------
int BS2Receiver::stopReceive()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::stopReceive");

    m_abort = true;
    m_recvEvent.signal();   // signal to wait in receive method.

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Dummy parse proc.
//-----------------------------------------------------------------------------
int BS2Receiver::parse(BCHAR* data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::parse");

	BS2BlockHeader			beforeHeader;
	BS2TransactionManager*	trmgr		= m_device->getTransactionManager();
    BS2BlockHeader*			curHeader	= (BS2BlockHeader *)data;
    BS2OStream*				ostmbuf;
    BS2TransactionInfo*		trinfo;
    BS2MessageInfo*			evtinfo;
    BS2MessageInfo			msgcont;
    BS2Message*				msg;
    BS2S9F7Message*			s9f7;
    BS2Sender*				sender = m_device->getSender();
	UINT					t_id;

	if (curHeader->getStreamNum() == 9 && curHeader->getFunctionNum() == 9)
    {
        this->parseStreamNine(data, size);   // case S9F9, timer cancel
        return BEE_ERROR;
    }

    if (curHeader->isPrimary())
    {
        trinfo = trmgr->buffer(curHeader, TRANSACTION_RECV_PRIMARY);
        while ((unsigned long)trinfo == 0xFFFFFFFF)
        {
            TRACE_ERROR((_TX("Tansaction-buffer locked(1).\n")));
            trinfo = trmgr->buffer(curHeader, TRANSACTION_RECV_PRIMARY);
        }
    }
    else
    {
        trinfo = trmgr->buffer(curHeader);
        while ((unsigned long)trinfo == 0xFFFFFFFF)
        {
            TRACE_ERROR((_TX("Tansaction-buffer locked(2).\n")));
            trinfo = trmgr->buffer(curHeader);
        }
    }
    if (trinfo != NULL)                 // Expected block ?
    {
        ostmbuf = trinfo->buffer();
        if (trinfo->type() == TRANSACTION_SEND_PRIMARY)
        {   // Secondary message
            ostmbuf->blockHeader(beforeHeader);
            if (beforeHeader.isExpect(curHeader) || (curHeader->getFunctionNum() == 0))
            {
                // Cancel reply timer
                this->cancelTimer(trinfo->timer_id());

                // First block of secondary message
                ostmbuf->reset();
                ostmbuf->write(data, size);
			
                msg = BS2MessageDictionary::instance()->make(ostmbuf);
                if (msg == NULL)
                {   // Unsupportted message
                    if (m_device->isEquipment())
                    {	
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }

                    evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                    if (evtinfo == NULL)
                    {
                        TRACE_ERROR((_TX("Tansaction-info locked(3).\n")));
                        TRACE_ERROR((_TX("Receive illegal data.\n")));
                        trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }
                    
					//this->notifyEventInfo(evtinfo);   // signal to wait in receiving method.
					this->putEventInfo(evtinfo);
                    
					TRACE_ERROR((_TX("Receive illegal data.\n")));
                    trmgr->remove(trinfo);
                    return BEE_ERROR;
                }
                else
                {
					//add by slime
					t_id = MAKE_TRANSACTION_IDENTIFIER(	curHeader->getSourceNum(),
														curHeader->getTransactionNum());

					if(t_id != trinfo->self())
					{
						TRACE_ERROR((_TX("Tansaction-info locked(7).\n")));
						return BEE_ERROR;
					}
					//end by slime

                    if (msg->noise())
                    {
                        TRACE_ERROR((_TX("Receive noise message data.\n")));
                        if (m_device->isEquipment())
                        {
                            s9f7 = new BS2S9F7Message(*curHeader);
                            sender->send(s9f7);
                            delete s9f7;
                        }
                        delete msg;    // Remove Noise

                        evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                        if (evtinfo == NULL)
                        {
                            TRACE_ERROR((_TX("Tansaction-info locked(4).\n")));
                            trmgr->remove(trinfo);
                            return BEE_ERROR;
                        }
                        
						//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
						this->putEventInfo(evtinfo);
                        
						//mark by slime, trmgr->info has deleted trinfo
						//trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }

                    // Get MsgControl and Removed trinfo
                    evtinfo = trmgr->info(trinfo, BS2RET_NORMAL, msg);
                    if (evtinfo == NULL)
                    {
                        TRACE_ERROR((_TX("Tansaction-info locked(5).\n")));
                        trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }
                    
					//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
					this->putEventInfo(evtinfo);
                }
            }
            else
            {   // unexpeted block
                TRACE_ERROR((_TX("HSMS does not support multi block.\n%s\n"),
                             _TX("Mismatched transaction number.")));
                TRACE_ERROR((_TX("Probably, received primary message is in losted trasaction number.\n")));
                trinfo->dump();
                if (trmgr->remove(trinfo) < 0)
                {
                    TRACE_ERROR((_TX("Transaction-remove locked(6). \n")));
                }
                return BEE_ERROR;
            }
        }
        else
        {   // Found multi-block of primary
            TRACE_ERROR((_TX("HSMS: Not multi block.\n")));
            beforeHeader.dump();
            return BEE_ERROR;
        }
    }
    else
    {   // Not multi-block message
        if (curHeader->getBlockNum() <= 1)
        {   // First block of primary
            ostmbuf = new BS2OStream;
            ostmbuf->write(data, size);

            msg = BS2MessageDictionary::instance()->make(ostmbuf);
            
			if (msg == NULL)
            {   
				// Un-supportted message
                if (m_device->isEquipment())
                {
                    s9f7 = new BS2S9F7Message(*curHeader);
                    sender->send(s9f7);
                    delete s9f7;
                }

				delete msg;
				delete ostmbuf;

                TRACE_ERROR((_TX("Don't make SECS-II message object.\n")));
                return BEE_ERROR;
            }
            else
            {   // Complete primary, message queued
                if (msg->noise())
                {
                    if (m_device->isEquipment())
                    {
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }

					delete msg;
					delete ostmbuf;
                    
					TRACE_ERROR((_TX("Illegal SECS-II message data.\n")));
                    return BEE_ERROR;
                }
                else
                {
                    evtinfo = new BS2MessageInfo;
                    //if (curHeader->wait())               // W bit on
                    //{   // Save system bytes for reply
                    trinfo = trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, evtinfo);
                    if (trinfo == NULL)
                    {
                        TRACE_ERROR((_TX("Can not create transaction info.\n")));
                        delete evtinfo;
                        delete ostmbuf;
                        return BEE_ERROR;            // Panic !
                    }
                    evtinfo = trinfo->eventHeader();
                    //}
					
                    evtinfo->setResult(BS2RET_NORMAL);
                    evtinfo->setMessage(msg);
                    
					//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
					this->putEventInfo(evtinfo);

                    if (curHeader->wait())      // W bit on
                    {   // Save system bytes for reply
                        // trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, NULL);
                        // * Saved above
                    }
                }
            
			}//end if (msg == NULL)
        }
        else
        {
            TRACE_ERROR((_TX("Don't found primary message.\n")));
            trmgr->dump();
            return BEE_ERROR;            // Discard block
        }
    }
    
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// parse stream-9
//-----------------------------------------------------------------------------
int BS2Receiver::parseStreamNine(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::parseStreamNine");

    BS2TransactionInfo*		trinfo;
    BS2MessageInfo*			evtinfo;
    BS2OStream*				ostmbuf;
    BS2Message*				msg;
	BS2TransactionManager*	trmgr	= m_device->getTransactionManager();
    BS2BlockHeader*			header	= (BS2BlockHeader *)data;
    BS2BlockHeader*			mhead;
    BS2ItemHeader*			itemhead = (BS2ItemHeader *)(data + sizeof(BS2BlockHeader));
    
    mhead = (BS2BlockHeader *)(itemhead->data());

    if (header->getFunctionNum() == 9 || header->getFunctionNum() == 13)
    {   // Conversation timeout
        trinfo = trmgr->buffer(mhead, TRANSACTION_RECV_PRIMARY);
        if (trinfo != NULL)
        {
            trmgr->remove(trinfo);

            // evtinfo = trmgr->info(trinfo, BS2MessageInfo::TIMEOUT_MESSAGE, NULL);
            // this->putEventInfo(evtinfo);
            // m_recvEvent.signal();   // signal to wait in receive method.
        }
        return 0;
    }
    else
    {
        trinfo = trmgr->buffer(mhead, TRANSACTION_SEND_PRIMARY);
        if (trinfo != NULL)
        {
            // Cancel reply timer
            this->cancelTimer(trinfo->timer_id());

            // Create S9F??
            ostmbuf = trinfo->buffer();
            ostmbuf->reset();
            ostmbuf->write(data, size);

            msg = BS2MessageDictionary::instance()->make(ostmbuf);
            if (msg == NULL)
            {   // Un-supportted message
                ACE_ERROR((LM_ERROR, ACE_TEXT("Receive illegal S9Fx data.\n")));
                // Remove the transaction
                trmgr->remove(trinfo);

                // evtinfo = trmgr->info(trinfo, BS2MessageInfo::ILLEGAL_MESSAGE, NULL);
                // this->putEventInfo(evtinfo);
                // m_recvEvent.signal();   // signal to wait in receive method.
            }
            else
            {
                evtinfo = trmgr->info(trinfo, BS2RET_NORMAL, msg);
                
				this->putEventInfo(evtinfo);
                m_recvEvent.signal();   // signal to wait in receive method.
            }
        }
        return 1;
    }
}


//-----------------------------------------------------------------------------
// Dispatch receive data.
//-----------------------------------------------------------------------------
int BS2Receiver::dispatch(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::dispatch");
    BS2BlockHeader * bheader = (BS2BlockHeader *)(data);
    int stnum = bheader->getStreamNum();
    int fcnum = bheader->getFunctionNum();

    // Check device ID.
    if ((bheader->getDeviceId() != m_device->getDriver()->m_deviceId) && (stnum != 9))
    {
        if (m_device->isEquipment())
        {
            BS2S9F1Message * s9f1 = new BS2S9F1Message(*bheader);
            m_device->getSender()->send(s9f1);
        }
        return BEE_SUCCESS;       // not send NAK
    }

    // Check stream number.
    if (! BS2MessageDictionary::instance()->existStreamNum(stnum))
    {
        if (m_device->isEquipment())
        {
            BS2S9F3Message * s9f3 = new BS2S9F3Message(*bheader);
            m_device->getSender()->send(s9f3);
        }
        return BEE_SUCCESS;       // not send NAK
    }

    // Check function number.
    if (BS2MessageDictionary::instance()->search(SFCODE(stnum, fcnum)) == NULL)
    {
        if (m_device->isEquipment())
        {
            BS2S9F5Message * s9f5 = new BS2S9F5Message(*bheader);
            m_device->getSender()->send(s9f5);
        }
        return BEE_SUCCESS;       // not send NAK
    }

    ACE_Message_Block * mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(size + 16), BEE_ERROR);
    memcpy(mb->rd_ptr(), data, size);
    mb->length(size);
    
	//this->put(mb);

	this->Dispatch_Block(mb); //rev then parse

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Dummy thread
//-----------------------------------------------------------------------------
int BS2Receiver::svc(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::svc");

    int					length;
	int					result		= 0;
    bool				receiving	= false;
    ACE_Time_Value		delaytm; //0.1 sec
	ACE_Message_Block*	mb;

	delaytm.set(ACE_OS::time(0), 500000);
	
	m_bs2receiver_loop = 1;
	
	while(m_bs2receiver_play)
    {
		result = this->getq(mb, &delaytm);
        		
		if( result >= 0 )
		{
			if (receiving)
			{
				this->ungetq(mb);
				continue;
			}

			length = mb->length();
			if (length > 0)
			{
				BCHAR* datap	= mb->rd_ptr();
				receiving		= true;

				result = this->parse(datap, length);   // block receive.
				if (result != BEE_SUCCESS)
				{
					TRACE_ERROR((_TX("Error message.\n")));
				}
				receiving = false;
			}
			mb->release();

			if (length == 0)             // shutdown
				break;
		}
    }

    if (result == -1 && errno == EWOULDBLOCK)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("(%t) %p\n"), ACE_TEXT("timed out waiting for message")));
    }

	m_bs2receiver_loop = 0;

    return 0;
}

//not for thread
int BS2Receiver::Dispatch_Block(ACE_Message_Block* mb)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::Dispatch_Block");

    int					length;
	int					result = 0;
	BCHAR*				datap;
	
	length = mb->length();
	
	if (length > 0)
	{
		datap		= mb->rd_ptr();

		result = this->parse(datap, length);   // block receive.
		
		if (result != BEE_SUCCESS)
		{
			TRACE_ERROR((_TX("Error message.\n")));
		}
	}
	
	mb->release();
	
    if (result == -1 && errno == EWOULDBLOCK)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("(%t) %p\n"), ACE_TEXT("timed out waiting for message")));
    }

    return 0;
}
