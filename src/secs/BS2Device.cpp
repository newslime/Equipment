// $Id: BS2Device.cpp,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Device.cpp
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

#include "BS2Device.h"
#include "BS2SECSReceiver.h"
#include "BS2HSMSReceiver.h"
#include "BS2Sender.h"
#include "BS2Serial.h"
#include "BS2Socket.h"
#include "BS2Driver.h"
#include "BS2TransactionManager.h"
#include "BS2MessageInfo.h"
#include "SECSXmlParser.h"

extern int	make_msgtype();   // see, BS2DefineMessage.cpp
extern void close_msgtype();   // see, BS2DefineMessage.cpp
//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Device::BS2Device()
: m_devid(0), 
  m_action(EQUIPMENT), 
  m_driver(NULL), 
  m_sender(NULL),
  m_receiver(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Device::BS2Device");
    
	//modified by slime
	m_trmgr = new BS2TransactionManager;
    //m_trmgr = BS2TransactionManager::instance();

	m_bs2device_play	= 0;
	m_bs2device_loop	= 0;
}

//-----------------------------------------------------------------------------
BS2Device::~BS2Device()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Device::~BS2Device");
    
    if (m_sender)
        delete m_sender;
	
	if (m_driver)
        delete m_driver;
	
	if (m_receiver)
        delete m_receiver;

	if (m_trmgr)
        delete m_trmgr;

	SECSXmlParser::close_singleton();
	//add by slime
	//m_trmgr->close_singleton();
	
	//modified by slime
	if(isEquipment())
		close_msgtype();
}

//-----------------------------------------------------------------------------
// Create new receiver and sender.
//-----------------------------------------------------------------------------
void BS2Device::hexDump(bool tf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::doDump");
    if (m_driver != NULL)
        m_driver->hexDump(tf);
}

//-----------------------------------------------------------------------------
// Create new receiver and sender.
//-----------------------------------------------------------------------------
//int initialize(DeviceParameter * parm);
int BS2Device::initialize(DeviceParameter * parm, BS2Sender* local_sender, BS2Receiver*	local_receiver)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::initialize");

    int result;
    ACE_TCHAR name[64];

    result = this->initxml(parm);
    if (result < 0)
    {
        return result;
    }

    //m_sender = new BS2Sender(this);
	if( local_sender )
		m_sender = local_sender;
	else
		m_sender = new BS2Sender(this);

    m_devid		= parm->m_deviceId;
    m_action	= (parm->m_slave == 0) ? EQUIPMENT : HOST;
    m_trmgr->sourceId(parm->m_sourceId);
    if (parm->m_dtype == DRIVER_SOCKET)
    {
        //BS2HSMSReceiver * hsms = new BS2HSMSReceiver(this);
        //m_receiver = hsms;
		if( local_receiver )
			m_receiver	= local_receiver;
		else
			m_receiver	= new BS2HSMSReceiver(this);
        
		BS2Socket* socket = new BS2Socket();
        m_driver = socket;
        ACE_OS::strcpy(name, "HSMS");
        m_sender->T6(((SocketParameter *)parm)->m_t6timeout);
    }
    else
    {
        //BS2SECSReceiver * secs1 = new BS2SECSReceiver(this);
        //m_receiver = secs1;
        if( local_receiver )
			m_receiver	= local_receiver;
		else
			m_receiver	= new BS2SECSReceiver(this);
		
		BS2Serial * serial = new BS2Serial();
        m_driver = serial;
        ACE_OS::strcpy(name, "SECS-I");
    }

    // initial control tables
    if (m_driver->initialize(parm, this) >= 0)
    {
        result = m_driver->open();
        if (result >= 0)
        {
            if (m_sender->open() >= 0)
            {
                m_sender->T3(parm->m_t3timeout);
                m_sender->setLinktestTime(parm->m_hbtimeout);
                if (m_receiver->open() >= 0)
                {
                    return BEE_SUCCESS;
                }
                else
                {
                    TRACE_ERROR((_TX("(%t) receiver is not opend %s %d\n"),
                                 name, parm->m_deviceId));
                }
            }
            else
            {
                TRACE_ERROR((_TX("(%t) sender is not opend %s %d\n"),
                             name, parm->m_deviceId));
            }
        }
        else
        {
            TRACE_ERROR((_TX("(%t) device module is not opened %s %d\n"),
                         name, parm->m_deviceId));
        }
    }
    else
    {
        TRACE_ERROR((_TX("(%t) device module is not initalized %s %d\n"),
                     name, parm->m_deviceId));
    }

    // fail open module.
    delete m_driver;
    m_driver = NULL;
    delete m_sender;
    m_sender = NULL;
    delete m_receiver;
    m_receiver = NULL;
    return BEE_ERROR;
}

//-----------------------------------------------------------------------------
// Get secs format table.
//-----------------------------------------------------------------------------
int BS2Device::initxml(DeviceParameter * parm)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::initxml");

	//modified by slime
	if(isEquipment())
		make_msgtype();  // Define default SECS Message Type

    int result = ACE_OS::access(parm->m_xmlname, R_OK);
    if (result >= 0)
    {
        TRACE_DEBUG((_TX("SECS Message Definition file is found (%s).\n"), parm->m_xmlname));
        
		//get xml define instead of default
		result = SECSXmlParser::instance()->parseMessage(parm->m_xmlname);
    }
    else
    {
        result = 0;
    }
    return result;
}

//-----------------------------------------------------------------------------
// Open driver.
//-----------------------------------------------------------------------------
int BS2Device::open(void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::open");

	m_bs2device_play = 1;

    if (this->activate(THR_NEW_LWP | THR_DETACHED) == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("spawn")));
        return BEE_ERROR;
    }

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Close
//-----------------------------------------------------------------------------
int BS2Device::close(int exit_status)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::close");

    if (exit_status < 0)
    {
		m_bs2device_play = 0;
		while( m_bs2device_loop )
		{
#if defined(_WIN32)
			::Sleep(200);
#else
			usleep(200000);
#endif
		}
				
		if (m_sender)
            m_sender->close();
		
		//do soekct looping stop inside
		if (m_driver)
            m_driver->close();

		if (m_receiver)
			m_receiver->close();
    }
    
	//TRACE_DEBUG((_TX("(%t) thread is exiting with status %d in module %s\n"),
    //             exit_status, this->getName()));

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Dummy parse proc.
//-----------------------------------------------------------------------------
int BS2Device::parse(void* data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Receiver::parse");
    
	return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Change device id.
//-----------------------------------------------------------------------------
int BS2Device::setDeviceId(int id)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::setDeviceId");
    // Caution! Not mutex lock.
    int retval = m_devid;
    m_devid = id;
    m_driver->deviceId(id);
    return retval;
}

//-----------------------------------------------------------------------------
// Restart receive interpreter.
//-----------------------------------------------------------------------------
int BS2Device::restart(void *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::restart");

    if (this->activate(THR_NEW_LWP | THR_DETACHED) == -1)
    {
        TRACE_ERROR((_TX("%p\n"), _TX("spawn")));
        return BEE_ERROR;
    }

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Disconnected communication.
//-----------------------------------------------------------------------------
int BS2Device::disconnected()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::disconnected");

    m_trmgr->removeAll();
    m_receiver->stopReceive();
    return true;
}

//-----------------------------------------------------------------------------
// Send message.
//-----------------------------------------------------------------------------
int BS2Device::send(BS2Message * msg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::send");

    int result;
    result = m_sender->send(msg, NULL);
    return result;
}

//-----------------------------------------------------------------------------
int BS2Device::send(BS2Message * msg, void * vptr)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::send");

    int result;
#if 0
    // Check multi block message
    // Not support, yet.
    if (msg->multi())
    {
        // Send Multi-Block Inquire
        BS2MultiBlock * multinq = new BS2MultiBlock(msg);
        BS2Message * inqmsg = this->addInquiry(multinq);
        if (inqmsg != NULL)
        {
            result = m_sender->send(inqmsg, vptr);
            multinq->inqNum(inqmsg->sf());
            multinq->tid(result);     // Save <tid>
        }
        else
        {   // Error making multi-block inquire
            delete multinq;
            result = m_sender->send(msg, vptr);
        }
    }
    else
#endif
    {
        result = m_sender->send(msg, vptr);
    }
    return result;
}

//-----------------------------------------------------------------------------
int BS2Device::send_s(BS2Message * msg, void * vptr)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::send_s");

    int result;
    result = m_sender->send(msg, vptr);
    return result;
}

//-----------------------------------------------------------------------------
// receive message.
//-----------------------------------------------------------------------------
int BS2Device::receive(BS2MessageInfo* ret, bool needtimeout)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::receive");

    int result;
    result = m_receiver->receive(ret, needtimeout);
    return result;
}

//-----------------------------------------------------------------------------
// receive message with limit time.
//-----------------------------------------------------------------------------
int BS2Device::receiveWithLimit(BS2MessageInfo* ret, ACE_Time_Value *tv)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::receiveWithLimit");

    int result;
    result = m_receiver->receiveWithLimit(ret, tv);
    return result;
}

//-----------------------------------------------------------------------------
// receive message.
//-----------------------------------------------------------------------------
int BS2Device::stop_receive()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::receive");

    int result;
    putCommand(NULL);
    result = m_receiver->stopReceive();
    return result;
}

//-----------------------------------------------------------------------------
// Cancel receive message.
//-----------------------------------------------------------------------------
int BS2Device::cancelReceive()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::cancelReceive");

    int result;

    result = m_receiver->stopReceive();
    return result;
}

//-----------------------------------------------------------------------------
// sense received message.
//-----------------------------------------------------------------------------
int BS2Device::sense(BS2MessageInfo* ret)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::sense");

    int result;
    result = m_receiver->sense(ret);
    return result;
}

//-----------------------------------------------------------------------------
// Simply enqueue the Message_Block into the end of the queue.
//-----------------------------------------------------------------------------
int BS2Device::put(ACE_Message_Block *mb, ACE_Time_Value *tv)
{
    return this->putq(mb, tv);
}

//-----------------------------------------------------------------------------
// Save multi message info.
//-----------------------------------------------------------------------------
BS2Message * BS2Device::addInquiry(BS2MultiBlock * multinq)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::addInquiry");
    int errmsg = 0;
    ACE_Guard<ACE_Thread_Mutex> multi_mon(this->m_lock);
    while (multi_mon.locked() == 0)
    {   // if collision, monitor is pass
        if (errmsg != 0)
        {
            TRACE_DEBUG((_TX("Mutex is not locked.\n")));
            errmsg = 1;
        }
    }

    BS2Message * inqmsg = BS2Message::makeInquiry(multinq->message());
    if (inqmsg == NULL)
    {
        return NULL;
    }

    m_multi.push_back(multinq);             // Queue multi-block info.

    return inqmsg;
}

//-----------------------------------------------------------------------------
// Remove multi message info.
//-----------------------------------------------------------------------------
BS2Message * BS2Device::removeInquiry(BS2Message * grantmsg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Device::addInquiry");
    int errmsg = 0;
    ACE_Guard<ACE_Thread_Mutex> multi_mon(this->m_lock);
    while (multi_mon.locked() == 0)
    {   // if collision, monitor is pass
        if (errmsg != 0)
        {
            TRACE_DEBUG((_TX("Mutex is not locked.\n")));
            errmsg = 1;
        }
    }

    BS2MultiBlock * multinq;
    int sfnum = grantmsg->sf() - 1;        // set primary stream/function
    int transnum = grantmsg->transNum();   // set primary transaction
    list<BS2MultiBlock *>::iterator iter = m_multi.begin();
    for ( ; iter != m_multi.end(); iter++)
    {
        multinq = *iter;
        if (multinq->inqNum() == sfnum && multinq->trans() == transnum)
        {
            break;
        }
    }
    if (iter != m_multi.end())
    {
        return NULL;
    }

    m_multi.erase(iter);

    BS2Message * orgmsg = multinq->message(); // Get and Return original message
    delete multinq;
    return orgmsg;
}

//-----------------------------------------------------------------------------
// Put tcl command in thread interpreter
//-----------------------------------------------------------------------------
int BS2Device::putCommand(BCHAR * cmd)
{
    ACE_Message_Block *mb;

    if (cmd == NULL)
    {
        ACE_NEW_RETURN(mb, ACE_Message_Block(BUFSIZ + 1), -1);
        // Send a shutdown message to the other thread and exit.
        mb->length(0);
        if (this->put(mb) == -1)
            ACE_ERROR((LM_ERROR, "(%t) %p\n", "put"));
        return 0;
    }

    int len = ACE_OS::strlen(cmd) + 1;
    ACE_NEW_RETURN(mb, ACE_Message_Block(len + 128), -1);
    ACE_OS::memcpy(mb->rd_ptr(), cmd, len);

    mb->length(len);
    // NUL-terminate the string (since we use strlen() on it later).

    if (this->put(mb) == -1)
        ACE_ERROR ((LM_ERROR, "(%t) %p\n", "put_next"));

    return 0;
}

//-----------------------------------------------------------------------------
// Receive message thread.
//-----------------------------------------------------------------------------
int BS2Device::svc(void)
{
	int	result;
	
	m_bs2device_loop = 1;
    
	while( m_bs2device_play )
    {
		result = this->parse(NULL, 0);
    }

    ACE_DEBUG((LM_DEBUG, "(%t) %s \"device %d\" dispatcher is deleted.\n", this->getName(), m_devid));

	m_bs2device_loop = 0;

    return 0;
}