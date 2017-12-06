// $Id: BS2MessageDictionary.cpp,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2MessageDictionary.cpp
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

#include "BS2Stream.h"
#include "BS2MessageDictionary.h"
#include "BS2NoiseMessage.h"
#include "BS2Interpreter.h"

const int MAX_STREAM_COUNT = 128;

typedef BS2Messages::value_type BS2MsgPair;

// Process-wide Item Dictionary.
BS2MessageDictionary * BS2MessageDictionary::_msg_dict = NULL;

// Controls whether the BS2MessageDictionary is deleted when we shut down
int BS2MessageDictionary::_delete_msg_dict = 0;

static BCHAR _supportStream[MAX_STREAM_COUNT] = {0};

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2MessageDictionary::~BS2MessageDictionary()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2MessageDictionary::BS2MessageDictionary");

    this->close();
}

//------------------------------------------------------------------------------
// Get Item Dictonary.
//-----------------------------------------------------------------------------
BS2MessageDictionary * BS2MessageDictionary::instance(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::instance");

    if (BS2MessageDictionary::_msg_dict == 0)
    {   // Perform Double-Checked Locking Optimization.
//#if defined(_WIN32)	        
//		ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance(), 0));
//#endif

        if (BS2MessageDictionary::_msg_dict == 0)
        {
            ACE_NEW_RETURN(BS2MessageDictionary::_msg_dict, BS2MessageDictionary, 0);
            BS2MessageDictionary::_delete_msg_dict = 1;
        }
    }
    return BS2MessageDictionary::_msg_dict;
}

//------------------------------------------------------------------------------
void BS2MessageDictionary::close_singleton(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::close_singleton");

//#if defined(_WIN32)	    
//	ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance()));
//#endif

    if (BS2MessageDictionary::_delete_msg_dict)
    {
        BS2MessageDictionary::_msg_dict->close();
        delete BS2MessageDictionary::_msg_dict;
        BS2MessageDictionary::_msg_dict = NULL;
        BS2MessageDictionary::_delete_msg_dict = 0;
    }
}

//------------------------------------------------------------------------------
// Close up and release all resources.
//------------------------------------------------------------------------------
int BS2MessageDictionary::close(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::close");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, -1));

    BS2MessageType * msg;
    if (m_messages.size() > 0)
    {
        BS2Messages::iterator iter;
        for (iter = m_messages.begin(); iter != m_messages.end(); ++iter)
        {
            msg = (BS2MessageType *)(*iter).second;
            delete msg;
        }
        m_messages.erase(m_messages.begin(), m_messages.end());
    }
    return 0;
}

//-----------------------------------------------------------------------------
// check to support stream number.
//-----------------------------------------------------------------------------
bool BS2MessageDictionary::existStreamNum(int n)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::existStreamNum");

    if (0 >= n || n >= MAX_STREAM_COUNT)
        return false;

    return (_supportStream[n] != 0);
}

//-----------------------------------------------------------------------------
// Get Spooling Stream Function Numbers.
//-----------------------------------------------------------------------------
void BS2MessageDictionary::getSpoolingStreams(StreamFunctionNumbers& sfnums)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::getSpoolingStreams");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock));

    BS2MessageType * msg;
    if (m_messages.size() > 0)
    {
        BS2Messages::iterator iter;
        for (iter = m_messages.begin(); iter != m_messages.end(); ++iter)
        {
            msg = (BS2MessageType *)(*iter).second;
            if (msg->isSecondary() || (msg->sfnum() == SFCODE(1,1)) ||
                (msg->sfnum() == SFCODE(1,13)))
            {
                continue;
            }
            sfnums.push_back(msg->sfnum());
        }
        std::sort(sfnums.begin(), sfnums.end(), less<int>());
    }

    return ;
}

//-----------------------------------------------------------------------------
// Add new item.
//-----------------------------------------------------------------------------
BS2MessageType* BS2MessageDictionary::add(BS2MessageType* type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::add");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    UINT			sfnum	= type->sfnum();
    BS2MessageType* oldtype = NULL;
    
	BS2Messages::iterator iter = m_messages.find(sfnum);
    if (iter != m_messages.end())
    {
        oldtype = (BS2MessageType *)(*iter).second;
		
		oldtype->EraseList(); //by stone
        
		delete oldtype;
		m_messages.erase(iter);
        m_messages.insert(BS2MsgPair(sfnum, type));
        //delete oldmsg;
    }
    else
    {
        _supportStream[STREAMCODE(sfnum) & 0x7F]++;
        m_messages.insert(BS2MsgPair(sfnum, type));
    }
    return NULL;
}

//-----------------------------------------------------------------------------
// Erase Message.
//-----------------------------------------------------------------------------
int BS2MessageDictionary::erase(UINT sfnum)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::erase");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, -1));

    BS2MessageType * type = NULL;
    BS2Messages::iterator iter = m_messages.find(sfnum);
    if (iter != m_messages.end())
    {
        type = (BS2MessageType *)(*iter).second;

		type->EraseList(); //by stone

        delete type;
		m_messages.erase(iter);
        //delete msg;
        
		_supportStream[STREAMCODE(sfnum) & 0x7F]--;
    }
    
	return 0;
}

//-----------------------------------------------------------------------------
void BS2MessageDictionary::eraseAll()
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::eraseAll");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock));

    BS2MessageType* type;

	//bug, crash
    /*if (m_messages.size() > 0)
    {
        BS2Messages::iterator iter;
        for (iter = m_messages.begin(); iter != m_messages.end(); ++iter)
        {
            msg = (BS2MessageType *)(*iter).second;
            delete msg;
            m_messages.erase(iter);
        }

        for (int i = 0; i < MAX_STREAM_COUNT; i++)
        {
            _supportStream[i] = 0;
        }
    }

    return;*/

	unsigned int			i;
	unsigned int			size;
	BS2Messages::iterator	iter;
		
	if ( m_messages.size() > 0 )
    {
		iter = m_messages.begin();
		size = m_messages.size();
		
		for(i=0; i<size; i++)
		{
			type = (BS2MessageType *)(*iter).second;
			
			type->EraseList(); //by stone
			
			delete type;
			m_messages.erase(iter);
			iter = m_messages.begin();
		}

        for (i = 0; i < MAX_STREAM_COUNT; i++)
        {
            _supportStream[i] = 0;
        }
    }
}


//-----------------------------------------------------------------------------
// search type message object by stream-function number
//-----------------------------------------------------------------------------
BS2MessageType * BS2MessageDictionary::search(UINT sf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::search");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2Messages::iterator iter = m_messages.find(sf);
    BS2MessageType * result = NULL;
    if (iter != m_messages.end())
    {
        result = (BS2MessageType *)(*iter).second;
    }
    return result;
}

//-----------------------------------------------------------------------------
// create message object by stream-function number
//-----------------------------------------------------------------------------
BS2Message * BS2MessageDictionary::factory(UINT sf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::factory");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2Messages::iterator iter = m_messages.find(sf);
    BS2Message*		msg		= NULL;
    BS2MessageType* type	= NULL;
    if (iter != m_messages.end())
    {
        type = (BS2MessageType *)(*iter).second;
        BS2Assert(type != NULL);
        msg = type->factory();
    }
    
	return msg;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BS2Message * BS2MessageDictionary::make(BS2OStream * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::make");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2Message * result;
    BS2MessageType * msgtype;
    UINT sfnum;
    BS2Message dummy;
    BS2IStream * inbuf = buf->make();

    inbuf->reset();
    *inbuf >> dummy;
    sfnum = dummy.sf();
#ifdef USE_MULTIMAP
    BS2Messages::iterator iter = m_messages.find(sfnum);
    if (iter == m_messages.end())
#else
    msg = m_messages[sfnum];
    if (msg == NULL)
#endif
    {
        TRACE_ERROR((_TX("illegal stream/function (0x%x).\n"), sfnum));
        result = (BS2Message *)(new BS2NoiseMessage(inbuf));
        delete inbuf;
        return result;
    }
#ifdef USE_MULTIMAP
    else
    {
        size_t snum = m_messages.count(sfnum);
        iter = m_messages.lower_bound(sfnum);
        for (size_t n = 0; n < snum; n++)
        {
            msgtype = (*iter).second;
            if ((result = msgtype->factory(inbuf)) != NULL)
            {   // format is match
                break;
            }
            iter++;    // next type of same stream-function
        }
        if (result == NULL)
        {
            result = (BS2Message *)(new BS2NoiseMessage(inbuf));
        }
    }
#else

    if ((result = msg->factory(inbuf)) == NULL)
    {
        result = (BS2Message *)(new BS2NoiseMessage(inbuf));
    }
#endif
    delete inbuf;
    return result;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2MessageDictionary::print(BS2InterpBase * interp)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageDictionary::print");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock));

    BS2MessageType * msg;
    if (m_messages.size() > 0)
    {
        BS2Messages::const_iterator iter;
        for (iter = m_messages.begin(); iter != m_messages.end(); ++iter)
        {
            msg = (BS2MessageType *)(*iter).second;
            msg->print(interp);
        }
    }

    return ;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2MessageDictionary::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "C::dump");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon,
                     const_cast<ACE_Recursive_Thread_Mutex&>(this->m_lock)));

    BS2MessageType * msg;
    if (m_messages.size() > 0)
    {
        BS2Messages::const_iterator iter;
        for (iter = m_messages.begin(); iter != m_messages.end(); ++iter)
        {
            msg = (BS2MessageType *)(*iter).second;
            msg->dump();
        }
    }

    return ;
}


//
// *** End of File ***

