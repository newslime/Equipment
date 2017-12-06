// $Id: BS2TransactionInfo.cpp,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TransactionInfo.cpp
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

#include "BS2TransactionInfo.h"
#include "BS2TransactionManager.h"
#include "BS2Stream.h"
#include "BS2MessageInfo.h"

//-----------------------------------------------------------------------------
//
// Transaction information
//
//-----------------------------------------------------------------------------
BS2TransactionInfo::BS2TransactionInfo(void)
        : m_transid(0), m_type(TRANSACTION_UNKNOWN), m_registered(0),
          m_timerid(-1), m_tm(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TransactionInfo::BS2TransactionInfo");

    ACE_NEW(this->m_sync, ACE_DEFAULT_THREAD_MANAGER_LOCK);
    m_sbuf = NULL;
    m_evtinfo = NULL;
}

//-----------------------------------------------------------------------------
BS2TransactionInfo::BS2TransactionInfo(int tid, BS2OStream * stmbuf,
                                       BS2MessageInfo * evtinfo)
        : m_transid(tid), m_type(TRANSACTION_UNKNOWN), m_registered(0),
          m_timerid(-1), m_tm(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TransactionInfo::BS2TransactionInfo");

    ACE_NEW(this->m_sync, ACE_DEFAULT_THREAD_MANAGER_LOCK);
    m_sbuf = stmbuf;
    m_sbuf->blockHeader(m_header);
    
	//if (evtinfo != NULL)
	if ( evtinfo )
    {
        evtinfo->setTransactionID(tid);
    }
    
	m_evtinfo = evtinfo;
}

//-----------------------------------------------------------------------------
BS2TransactionInfo::~BS2TransactionInfo(void)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TransactionInfo::~BS2TransactionInfo");
    delete this->m_sync;
    
	if (m_sbuf)
    {
        delete m_sbuf;
		m_sbuf = NULL;
    }
    
	if (m_evtinfo)
    {
        delete m_evtinfo;
		m_evtinfo = NULL;
    }
}

//-----------------------------------------------------------------------------
void BS2TransactionInfo::restoreHeader()
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionInfo::restoreHeader");

    this->m_sync->acquire();
    if (m_sbuf)
    {
        m_sbuf->blockHeader(m_header);
    }
    this->m_sync->release();
}

void BS2TransactionInfo::dump(void) const
{
    ACE_DEBUG((LM_DEBUG, ACE_BEGIN_DUMP, this));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT(" * [m_transid = %d, m_type = %d]"),
                                  this->m_transid, this->m_type));
#if 0
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_registered = %d"), this->m_registered));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT(", m_tm = 0x%X"), this->m_tm));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT(", m_sync = 0x%X"), this->m_sync));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT(", m_sbuf = 0x%X"), this->m_sbuf));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT(", m_evtinfo = 0x%X"), this->m_evtinfo));
#endif
    m_header.dump();
    ACE_DEBUG((LM_DEBUG, ACE_END_DUMP));
}

