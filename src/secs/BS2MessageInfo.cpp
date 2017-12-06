// $Id: BS2MessageInfo.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2MessageInfo.cpp
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

#include "BS2MessageInfo.h"
#include "BS2Message.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2MessageInfo::BS2MessageInfo(int result, BS2Message * msg)
: m_status(0), m_result(result), m_tid(0), m_del(false), m_userptr(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2MessageInfo::BS2MessageInfo");
    m_msg = msg;
    if (msg != NULL)
    {
        m_tid = msg->transNum();
    }
}

BS2MessageInfo::~BS2MessageInfo()
{
	//BS2MessageType*	msg_type;
	if( m_msg )
	{
		//can not delete, address in BS2MessageDictionary instance
		//msg_type = m_msg->messageType();
		//msg_type->EraseList();

		delete m_msg;		
		m_msg = NULL;
	}
}

//----------------------------------------------------------------------------
// Move result to application area.
//-----------------------------------------------------------------------------
BS2MessageInfo& BS2MessageInfo::operator=(const BS2MessageInfo& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageInfo::operator=");
    if (this == &rhs)
        return *this;
    copy(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
void BS2MessageInfo::copy(const BS2MessageInfo& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageInfo::copy");

    m_result = rhs.m_result;
    m_tid = rhs.m_tid;
    m_del = rhs.m_del;
    m_msg = const_cast<BS2MessageInfo&>(rhs).getMessageAndClear();   // narrow copy

    m_status = rhs.m_status;
    m_userptr = rhs.m_userptr;
    //return ;
}

//----------------------------------------------------------------------------
// Move result to application area.
//-----------------------------------------------------------------------------
void BS2MessageInfo::moveto(BS2MessageInfo* dst)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageInfo::moveto");

    dst->m_result	= m_result;
    dst->m_tid		= m_tid;
    dst->m_del		= m_del;
    dst->m_msg		= getMessageAndClear(); //move to new message pointer
	
    dst->m_status	= m_status;
    dst->m_userptr	= m_userptr;
    //return ;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2MessageInfo::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n* BS2MessageInfo\n"), this->m_status));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_status = %d, "), this->m_status));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_result = %d, "), this->m_result));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_tid = 0x%X, "), this->m_tid));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_del = %d, "), this->m_del));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_msg = %x, "), this->m_msg));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("m_userptr = %x\n"), this->m_userptr));
}

BS2Message* BS2MessageInfo::getMessageAndClear() 
{ 
	BS2Message* retain_msg;

	retain_msg	= m_msg; 
	m_msg		= NULL;
	
	return retain_msg;
}
