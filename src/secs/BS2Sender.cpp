// $Id: BS2Sender.cpp,v 1.9 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Sender.cpp
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

#include "ace/Auto_Ptr.h"
#include "BS2Device.h"
#include "BS2Sender.h"
#include "BS2Driver.h"
#include "BS2Message.h"
#include "BS2Stream.h"
#include "BS2Receiver.h"
#include "BS2MessageInfo.h"


struct Transaction_t
{
    BS2OStream    * m_sbuf;
    BS2MessageInfo * m_evtinfo;
    BS2TransactionInfo * m_transinfo;
};

BS2Sender::BS2Sender(BS2Device * device)
{
	m_T3				= 0; 
	m_T6				= 0;
	m_timeout			= 0; 
	m_testime			= 0;
	m_device			= device;
	m_bs2sender_play	= 0;
	m_bs2sender_loop	= 0;
}

BS2Sender::~BS2Sender()
{
	m_bs2sender_play = 0;
	while( m_bs2sender_loop )
	{
#if defined(_WIN32)
		::Sleep(200);
#else
		usleep(200000);
#endif
	}
}
//-----------------------------------------------------------------------------
// Spawn off a new thread.
//-----------------------------------------------------------------------------
int BS2Sender::open(void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::open");

    /*m_bs2sender_play = 1;
	
	if (this->activate(THR_NEW_LWP | THR_DETACHED) == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("spawn")));
        return -1;
    }*/
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Sender::close(u_long exit_status)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::close");

    //TRACE_DEBUG((_TX("(%t) thread is exiting with status %d.\n"), exit_status));

    return 0;
}

//-----------------------------------------------------------------------------
// Dummy parse proc.
//-----------------------------------------------------------------------------
int BS2Sender::parse(BCHAR *, int)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::parse");
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Simply enqueue the Message_Block into the end of the queue.
//-----------------------------------------------------------------------------
int BS2Sender::put(ACE_Message_Block *mb, ACE_Time_Value *tv)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::put");
    return this->putq(mb, tv);
}

//-----------------------------------------------------------------------------
// Send message
//-----------------------------------------------------------------------------
int BS2Sender::send(BS2Message * msg, void * ptr)
{
    bool		result;
	BS2OStream* outbuf;
	
	TRACE_FUNCTION(TRL_LOW, "BS2Sender::send");
	
    if (! m_device->getDriver()->canSend())
    {
        TRACE_ERROR((_TX("driver can not send message.\n")));
        return -1;
    }
    
	outbuf = new BS2OStream;
    result = outbuf->set(msg);
    
	if (result == true)
    {
        //return this->send(outbuf, ptr);
		return this->dispatch(outbuf, ptr);
    }
    else
    {
        TRACE_ERROR((_TX("%T secs stream buffer making error.\n")));
        return -1;
    }
}

//-----------------------------------------------------------------------------
//int BS2Sender::send(BS2OStream * outbuf, void * ptr)
int BS2Sender::dispatch(BS2OStream* outbuf, void* ptr)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::send");

    int						t_id = -1;
    BS2BlockHeader			header;
	ACE_Message_Block*		mb;
    Transaction_t*			buf_handle;
    BS2BlockHeader*			hptr;
    BS2TransactionInfo*		trinfo	= NULL;
    BS2TransactionManager*	trmgr	= m_device->getTransactionManager();
    BS2MessageInfo*			evtinfo = NULL;

    hptr = (BS2BlockHeader *)outbuf->ptop();
    
	/*if (hptr->isSession())
    {   // HSMS only
    
	}
    else if (outbuf->isPrimary())
    {
        evtinfo = new BS2MessageInfo(BS2RET_NORMAL, NULL);
        evtinfo->setUserPtr(ptr);
        if (outbuf->isWait())
        {   // queue expect responce
            trinfo = trmgr->insert(TRANSACTION_SEND_PRIMARY, outbuf,
                                   evtinfo);
            if (trinfo == NULL)
            {
                TRACE_ERROR((_TX("not entry transaction. \n")));
                delete outbuf;
                delete evtinfo;
                return -1;
            }
            evtinfo->setTransactionID(trinfo->self());
            t_id = (int)(trinfo->self() & 0xFFFF);
        }
    }
    else
    {
        trinfo = trmgr->buffer(outbuf, TRANSACTION_RECV_PRIMARY);
        if (trinfo == NULL)
        {
            TRACE_ERROR((_TX("Can't find transaction of received primary.\n")));
            outbuf->blockHeader(header);
            t_id = MAKE_TRANSACTION_IDENTIFIER(0, header.getTransactionNum());
        }
        else
        {
            t_id = (int)(trinfo->self() & 0xFFFF);
            evtinfo = trinfo->eventHeader();
        }
    }*/

	if ( !hptr->isSession() )
	{
		if (outbuf->isPrimary()  )
		{
			evtinfo = new BS2MessageInfo(BS2RET_NORMAL, NULL);
			evtinfo->setUserPtr(ptr);
			
			if (outbuf->isWait())
			{   // queue expect responce
				trinfo = trmgr->insert(TRANSACTION_SEND_PRIMARY, outbuf, evtinfo);
				if (trinfo == NULL)
				{
					TRACE_ERROR((_TX("not entry transaction. \n")));
					delete outbuf;
					delete evtinfo;
					return -1;
				}
				evtinfo->setTransactionID(trinfo->self());
				t_id = (int)(trinfo->self() & 0xFFFF);
			}
		}
		else
		{
			trinfo = trmgr->buffer(outbuf, TRANSACTION_RECV_PRIMARY);
			if (trinfo == NULL)
			{
				TRACE_ERROR((_TX("Can't find transaction of received primary.\n")));
				outbuf->blockHeader(header);
				t_id = MAKE_TRANSACTION_IDENTIFIER(0, header.getTransactionNum());
			}
			else
			{
				t_id = (int)(trinfo->self() & 0xFFFF);
				evtinfo = trinfo->eventHeader();
			}
		}
	}

    mb = new ACE_Message_Block(sizeof(Transaction_t) + 16);
    if (mb == NULL)
    {
        TRACE_ERROR((_TX("Can't allocate message block.\n")));
        errno = ENOMEM;                         // Can't allocate message block.
        if (trinfo != NULL)
        {
            if (trmgr->remove(trinfo) < 0)
            {
                TRACE_ERROR((_TX("transaction-remove unlocked. \n")));
            }
        }
        else
        {
            delete outbuf;
            delete evtinfo;
        }
        return -1;
    }

    buf_handle = (Transaction_t *)mb->rd_ptr();
    buf_handle->m_sbuf		= outbuf;
    buf_handle->m_evtinfo	= evtinfo;				
    buf_handle->m_transinfo = trinfo;
    mb->length(sizeof(Transaction_t));
    
	//this->put(mb);

	this->Dispatch_Block(mb);
    
	return t_id;
}

//-----------------------------------------------------------------------------
// Dummy thread
int BS2Sender::svc(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::svc");

    UINT					t_id;
    int						length;
	int						result		= 0;
    bool					sending		= false;
	ACE_Time_Value			delaytm; //0.1 sec
    BS2OStream*				ostmbuf;
    ACE_Message_Block*		mb;
    BS2BlockHeader			header;
    BS2BlockHeader*			hptr;
    BS2TransactionInfo*		trinfo;
	Transaction_t*			buf_handle;
	BS2OStream*				primary_buf;
	char*					datap; //for debug
    BS2Receiver*			receiver	= m_device->getReceiver();
    BS2Driver*				driver		= m_device->getDriver();

	//modified by slime
	BS2TransactionManager*	trmgr		= m_device->getTransactionManager();//BS2TransactionManager::instance();
    
	delaytm.set(ACE_OS::time(0), 500000);
	
	m_bs2sender_loop = 1;
    
	while( m_bs2sender_play )
    {
		result = this->getq(mb, &delaytm);
        		
		if( result >= 0 )
		{
			if (sending)
			{
				this->ungetq(mb);
				TRACE_ERROR((_TX("Sending, busy[2] !\n")));
				continue;
			}
	        
			length = mb->length();
			
			if (length > 0)
			{
				buf_handle	= (Transaction_t *)mb->rd_ptr();
				datap		= mb->rd_ptr(); //for debug look
				ostmbuf		= buf_handle->m_sbuf;
				hptr		= (BS2BlockHeader *)ostmbuf->ptop();
				sending		= true;

				result		= this->parse(datap, length);   // block sender.

				if (hptr->isSession())
				{   // HSMS only
					if (hptr->wantSessionRsp())
					{
						// queue expect responce
						trinfo = trmgr->insert(	TRANSACTION_SEND_SESSION, 
												ostmbuf,
												buf_handle->m_evtinfo );
						if (trinfo == NULL)
						{
							TRACE_ERROR((_TX("Not entry transaction. \n")));
							delete ostmbuf;
							delete buf_handle->m_evtinfo;
						}
						else
						{
							// T6 timer start.
							//receiver->addTimer(m_T6, trinfo);
							if (driver->send(trinfo) != BEE_SUCCESS)  // %%% S e n d %%%
							{   
								// T6 timer stop.
								//receiver->cancelTimer(trinfo->timer_id());
								
								if (trmgr->remove(trinfo) < 0)
								{
									TRACE_ERROR((_TX("Transaction-remove locked(2). \n")));
								}
							}
						}
					}
					else
					{
						ostmbuf->blockHeader(header);
	                    
						t_id	= MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(), header.getTransactionNum());
						
						trinfo	= new BS2TransactionInfo(t_id, ostmbuf, buf_handle->m_evtinfo);
						trinfo->type(TRANSACTION_SEND_SESSION);
	                    
						result	= driver->send(trinfo);   // %%% S e n d %%%
	                    
						delete trinfo;
					}
				}
				else if (ostmbuf->isPrimary())
				{
					bool reply_bit = ostmbuf->isWait();
	                
					if ((trinfo = buf_handle->m_transinfo) == NULL)
					{
						if (reply_bit == true)
						{   // Notify error.
							TRACE_ERROR((_TX("Not found send primary transaction. \n")));
							
							//this->receive of GemDevice::parse will get event
							receiver->notifyEventInfo(buf_handle->m_evtinfo);
							delete ostmbuf;
						}
						else
						{   // Maybe spooled message
							// TRACE_ERROR((_TX("Send spooled message. \n")));
							ostmbuf->blockHeader(header);
							
							t_id = MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(),
															   header.getTransactionNum());
							// Create temporary info.
							trinfo = new BS2TransactionInfo(t_id, 
															ostmbuf,
															buf_handle->m_evtinfo);
							
							trinfo->type(TRANSACTION_SEND_PRIMARY);
							result = driver->send(trinfo);  // %%% S e n d %%%
							delete trinfo;
						}
					}
					else
					{
						result = driver->send(trinfo); // %%% S e n d %%%
												
						trinfo = trmgr->buffer(ostmbuf, TRANSACTION_SEND_PRIMARY);
		
						if( trinfo )
							trmgr->remove(trinfo);
					}
				}
				else //RECV_PRIMARY
				{
					trinfo = trmgr->buffer(ostmbuf, TRANSACTION_RECV_PRIMARY);
	                
					if (trinfo == NULL)
					{
						TRACE_ERROR((_TX("Not find transaction of received primary. \n")));
						ostmbuf->blockHeader(header);
						
						t_id = MAKE_TRANSACTION_IDENTIFIER(	header.getSourceNum(),
															header.getTransactionNum() );
						// Create temporary info.
						trinfo = new BS2TransactionInfo(t_id, 
														ostmbuf,
														buf_handle->m_evtinfo);
						
						trinfo->type(TRANSACTION_RECV_PRIMARY);
						result = driver->send(trinfo); // %%% S e n d %%%
						
						delete trinfo;
					}
					else
					{
						primary_buf = trinfo->exchangeBuffer(ostmbuf);
						
						ostmbuf->blockHeader(header);
						
						t_id = MAKE_TRANSACTION_IDENTIFIER(	header.getSourceNum(),
															header.getTransactionNum() );

						result = driver->send(trinfo);   // %%% S e n d %%%
						
						if (result != BEE_SUCCESS)
						{
							TRACE_ERROR((_TX("Error send message. \n")));
						}
						
						delete primary_buf;
						
						if (trmgr->remove(trinfo) < 0)      // delete trinfo
						{
							TRACE_ERROR((_TX("Transaction-remove locked(5). \n")));
						}
					}
				}
			
				sending = false;
			}
			
			mb->release();

			if (length == 0)             // shutdown
				break;
		}
    }

    if (result == -1 && errno == EWOULDBLOCK)
    {
        ACE_ERROR((LM_ERROR,
            ACE_TEXT("(%t) %p\n"), ACE_TEXT("timed out waiting for message")));
    }

	m_bs2sender_loop = 0;

    return 0;
}

//not for thread
int BS2Sender::Dispatch_Block(ACE_Message_Block* mb)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Sender::Dispatch_Block");

    UINT					t_id;
    int						length;
	int						result		= 0;
    BS2OStream*				ostmbuf;
    BS2BlockHeader			header;
    BS2BlockHeader*			hptr;
    BS2TransactionInfo*		trinfo;
	Transaction_t*			buf_handle;
	BS2OStream*				primary_buf;
	char*					datap; //for debug
    BS2Receiver*			receiver	= m_device->getReceiver();
    BS2Driver*				driver		= m_device->getDriver();
	//modified by slime
	BS2TransactionManager*	trmgr		= m_device->getTransactionManager();//BS2TransactionManager::instance();
		    
	length = mb->length();
	
	if (length > 0)
	{
		buf_handle	= (Transaction_t *)mb->rd_ptr();
		datap		= mb->rd_ptr(); //for debug look
		ostmbuf		= buf_handle->m_sbuf;
		hptr		= (BS2BlockHeader *)ostmbuf->ptop();
		//sending		= true;

		result		= this->parse(datap, length);   // block sender.

		if (hptr->isSession())
		{   // HSMS only
			if (hptr->wantSessionRsp())
			{
				// queue expect responce
				trinfo = trmgr->insert(	TRANSACTION_SEND_SESSION, 
										ostmbuf,
										buf_handle->m_evtinfo );
				if (trinfo == NULL)
				{
					TRACE_ERROR((_TX("Not entry transaction. \n")));
					delete ostmbuf;
					delete buf_handle->m_evtinfo;
				}
				else
				{
					// T6 timer start.
					//receiver->addTimer(m_T6, trinfo);
					if (driver->send(trinfo) != BEE_SUCCESS)  // %%% S e n d %%%
					{   
						// T6 timer stop.
						//receiver->cancelTimer(trinfo->timer_id());
						
						if (trmgr->remove(trinfo) < 0)
						{
							TRACE_ERROR((_TX("Transaction-remove locked(2). \n")));
						}
					}
				}
			}
			else
			{
				ostmbuf->blockHeader(header);
                
				t_id	= MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(), header.getTransactionNum());
				
				trinfo	= new BS2TransactionInfo(t_id, ostmbuf, buf_handle->m_evtinfo);
				trinfo->type(TRANSACTION_SEND_SESSION);
                
				result	= driver->send(trinfo);   // %%% S e n d %%%
                
				delete trinfo;
			}
		}
		else if (ostmbuf->isPrimary())
		{
			bool reply_bit = ostmbuf->isWait();
            
			if ((trinfo = buf_handle->m_transinfo) == NULL)
			{
				if (reply_bit == true)
				{   // Notify error.
					TRACE_ERROR((_TX("Not found send primary transaction. \n")));
					
					//receiver->notifyEventInfo(buf_handle->m_evtinfo);
					delete ostmbuf;
					delete buf_handle->m_transinfo;
				}
				else
				{   // Maybe spooled message
					// TRACE_ERROR((_TX("Send spooled message. \n")));
					ostmbuf->blockHeader(header);
					
					t_id = MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(),
													   header.getTransactionNum());
					// Create temporary info.
					trinfo = new BS2TransactionInfo(t_id, 
													ostmbuf,
													buf_handle->m_evtinfo);
					
					trinfo->type(TRANSACTION_SEND_PRIMARY);
					result = driver->send(trinfo);  // %%% S e n d %%%
					delete trinfo;
				}
			}
			else
			{
				result = driver->send(trinfo); // %%% S e n d %%%
				
				trinfo = trmgr->buffer(ostmbuf, TRANSACTION_SEND_PRIMARY);

				if( trinfo )
					trmgr->remove(trinfo);
			}
		}
		else //RECV_PRIMARY
		{
			trinfo = trmgr->buffer(ostmbuf, TRANSACTION_RECV_PRIMARY);
            
			if (trinfo == NULL)
			{
				TRACE_ERROR((_TX("Not find transaction of received primary. \n")));
				ostmbuf->blockHeader(header);
				
				t_id = MAKE_TRANSACTION_IDENTIFIER(	header.getSourceNum(),
													header.getTransactionNum() );
				// Create temporary info.
				trinfo = new BS2TransactionInfo(t_id, 
												ostmbuf,
												buf_handle->m_evtinfo);
				
				trinfo->type(TRANSACTION_RECV_PRIMARY);
				result = driver->send(trinfo); // %%% S e n d %%%
				
				delete trinfo;
			}
			else
			{
				primary_buf = trinfo->exchangeBuffer(ostmbuf);
				
				ostmbuf->blockHeader(header);
				
				t_id = MAKE_TRANSACTION_IDENTIFIER(	header.getSourceNum(),
													header.getTransactionNum() );

				result = driver->send(trinfo);   // %%% S e n d %%%
				
				if (result != BEE_SUCCESS)
				{
					TRACE_ERROR((_TX("Error send message. \n")));
				}
				
				delete primary_buf;
				
				if (trmgr->remove(trinfo) < 0)      // delete trinfo
				{
					TRACE_ERROR((_TX("Transaction-remove locked(5). \n")));
				}
			}
		}
	}
	
	mb->release();

    if (result == -1 && errno == EWOULDBLOCK)
    {
        ACE_ERROR((LM_ERROR,
            ACE_TEXT("(%t) %p\n"), ACE_TEXT("timed out waiting for message")));
    }
	
    return 0;
}

