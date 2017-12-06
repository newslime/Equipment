// $Id: BS2TransactionManager.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TransactionManager.cpp
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

#include "BS2TransactionManager.h"
#include "BS2Stream.h"
#include "BS2MessageInfo.h"
#include "ace/Dynamic.h"
#include "ace/Object_Manager.h"
#include "ace/Singleton.h"

#if 0
#define GUARD_NOT_RETURN(MUTEX,OBJ,LOCK,RETURN)     ACE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN)
#define GUARD_BEE(MUTEX,OBJ,LOCK)                   ACE_GUARD(MUTEX,OBJ,LOCK)
#else
#define GUARD_NOT_RETURN(MUTEX,OBJ,LOCK,RETURN)  \
    ACE_Guard< MUTEX > OBJ (LOCK);               \
    int  ___##OBJ = 10;                          \
    while (1) {                                  \
        if (OBJ.locked () != 0)  break;          \
        ___##OBJ--;                              \
        if (___##OBJ == 0) return RETURN;        \
        ACE_OS::sleep(1);                        \
    }

#define GUARD_BEE(MUTEX,OBJ,LOCK)                \
    ACE_Guard< MUTEX > OBJ (LOCK);               \
    int  ___##OBJ = 10;                          \
    while (1) {                                  \
        if (OBJ.locked () != 0)  break;          \
        ___##OBJ--;                              \
        if (___##OBJ == 0) return ;              \
        ACE_OS::sleep(1);                        \
    }
#endif

// Process-wide Transaction Manager.
BS2TransactionManager * BS2TransactionManager::_trans_mgr = NULL;

// Controls whether the BS2TransactionManager is deleted when we shut down
// (we can only delete it safely if we created it!)
int BS2TransactionManager::_delete_trans_mgr = 0;


//-----------------------------------------------------------------------------
//
//  Transaction Manager
//
//-----------------------------------------------------------------------------
BS2TransactionManager * BS2TransactionManager::instance(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::instance");

    if (BS2TransactionManager::_trans_mgr == 0)
    {
        // Perform Double-Checked Locking Optimization.
//#if defined(_WIN32)	        
//		GUARD_NOT_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance(), 0);
//#endif

        if (BS2TransactionManager::_trans_mgr == 0)
        {
            ACE_NEW_RETURN(BS2TransactionManager::_trans_mgr, BS2TransactionManager, 0);
            BS2TransactionManager::_delete_trans_mgr = 1;
        }
    }

    return BS2TransactionManager::_trans_mgr;
}

//-----------------------------------------------------------------------------
BS2TransactionManager *
BS2TransactionManager::instance(BS2TransactionManager * tm)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::instance");

//#if defined(_WIN32)	
//    GUARD_NOT_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance(), 0);
//#endif

    BS2TransactionManager * t = BS2TransactionManager::_trans_mgr;
    // We can't safely delete it since we don't know who created it!
    BS2TransactionManager::_delete_trans_mgr = 0;
    BS2TransactionManager::_trans_mgr = tm;
    return t;
}

//-----------------------------------------------------------------------------
void BS2TransactionManager::close_singleton(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::close_singleton");

//#if defined(_WIN32)	
//    GUARD_BEE(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance());
//#endif

    if (BS2TransactionManager::_delete_trans_mgr)
    {
        BS2TransactionManager::_trans_mgr->close();
        delete BS2TransactionManager::_trans_mgr;
        BS2TransactionManager::_trans_mgr = 0;
        BS2TransactionManager::_delete_trans_mgr = 0;
    }
}

//-----------------------------------------------------------------------------
// Close up and release all resources.
//-----------------------------------------------------------------------------
int BS2TransactionManager::close()
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::close");
    // Clean up the thread descriptor list.
    //GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1); //removeAll have lock

    this->removeAll();
    return 0;
}

//-----------------------------------------------------------------------------
BS2TransactionInfo * BS2TransactionManager::buffer(BS2BlockHeader * header,
                                                   TransactionType type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::buffer");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock,
                     (BS2TransactionInfo *)0xFFFFFFFF);

    int tid = MAKE_TRANSACTION_IDENTIFIER(header->getSourceNum(),
                                          header->getTransactionNum());
    BS2TransactionInfo * ptr = this->find(tid, type);

    return ptr;
}

//-----------------------------------------------------------------------------
BS2TransactionInfo* BS2TransactionManager::buffer(BS2OStream * stmbuf,
                                                   TransactionType type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::buffer");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, (BS2TransactionInfo *)0xFFFFFFFF);

    BS2BlockHeader header;
    stmbuf->blockHeader(header);
    int tid = MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(),
                                          header.getTransactionNum());
    BS2TransactionInfo* ptr = this->find(tid, type);
    return ptr;
}

//-----------------------------------------------------------------------------
// Get transaction identifier and increment its.
//-----------------------------------------------------------------------------
UINT BS2TransactionManager::getTransactionId()
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::getTransactionId");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, 0);

    return this->transaction_id();
}

//-----------------------------------------------------------------------------
// Get transaction identifier and increment its.
//-----------------------------------------------------------------------------
// Must be called with locks held.
UINT BS2TransactionManager::transaction_id()
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::transaction_id");

    UINT t_id = GET_TRANSACTION_NUMBER(m_transaction_id);
    t_id++;
    if ((t_id & 0xFFFF) == 0)
    {
        t_id = 1;
    }
    m_transaction_id = MAKE_TRANSACTION_IDENTIFIER(m_srcid, t_id);
    return m_transaction_id;
}

//-----------------------------------------------------------------------------
// Append a transaction into the pool (does not check for duplicates).
//-----------------------------------------------------------------------------
// Must be called with locks held.
int BS2TransactionManager::append(UINT tid, TransactionType type,
                                  BS2TransactionInfo * ti)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::append");
    BS2TransactionInfo * trinfo;

    if (ti == 0)
    {
        ACE_NEW_RETURN(trinfo, BS2TransactionInfo, -1);
    }
    else
    {
        trinfo = ti;
    }
    trinfo->m_transid = tid;
    trinfo->m_type = type;
    trinfo->m_tm = this;               // Setup the Transaction Manager.

    this->m_transaction_list.insert_tail(trinfo);
    trinfo->m_registered = 1;
    return 0;
}

//-----------------------------------------------------------------------------
// Locate the index in the table associated with <tid>.
//-----------------------------------------------------------------------------
// Must be called with the lock held.
BS2TransactionInfo * BS2TransactionManager::find(UINT tid, TransactionType type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::find");

    TransactionInfoListIter iter(this->m_transaction_list);
    for ( ; ! iter.done(); iter.advance())
    {
        if (iter.next()->m_transid == tid)
        {
            if (type == TRANSACTION_UNKNOWN || type == iter.next()->m_type)
            {
                return iter.next();
            }
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
// Insert a transaction into the list.
//-----------------------------------------------------------------------------
// checks for duplicates and doesn't allow them to be inserted twice.
BS2TransactionInfo *
BS2TransactionManager::insert(TransactionType type, 
							  BS2OStream * stmbuf,
                              BS2MessageInfo * rtinfo)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::insert");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, NULL);

    // Check for duplicates and bail out if we're already registered...
    BS2BlockHeader header;
    stmbuf->blockHeader(header);
    UINT t_id = MAKE_TRANSACTION_IDENTIFIER(header.getSourceNum(),
                                            header.getTransactionNum());

    if (this->find(t_id, type) != NULL)
    {
        return NULL;
    }
    if (type != TRANSACTION_RECV_PRIMARY)
    {
        t_id = (UINT)this->transaction_id();
    }
    BS2TransactionInfo* ti;
    ACE_NEW_RETURN(ti, BS2TransactionInfo(t_id, stmbuf, rtinfo), NULL);
    
	if (this->append(t_id, type, ti) == -1)
    {
        TRACE_ERROR((_TX("can't entry transaction info. (%d)\n"), t_id));
        delete ti;
        return NULL;
    }
    else
    {
        return ti;
    }
}

//-----------------------------------------------------------------------------
int BS2TransactionManager::insert(UINT tid, TransactionType type,
                                            BS2TransactionInfo * ti)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::insert");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    // Check for duplicates and bail out if we're already registered...
    if (this->find(tid) != 0)
    {
        return BEE_ERROR;
    }
    return this->append(tid, type, ti);
}

//-----------------------------------------------------------------------------
// Get the infomation of transaction, and remove the transaction from the list.
//-----------------------------------------------------------------------------
BS2MessageInfo * BS2TransactionManager::info(BS2TransactionInfo * ti,
                                            int rc, BS2Message * msg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::result");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, NULL);

    BS2MessageInfo * info = ti->m_evtinfo;
    ti->m_evtinfo = NULL;

    info->setResult(rc);
    info->setMessage(msg);
    info->setDelete();

    ti->m_tm = NULL;
    this->m_transaction_list.remove(ti);
#if 0
    this->m_transinfo_freelist.add (ti);
#else
    delete ti;
#endif /* 0 */
    return info;
}

//-----------------------------------------------------------------------------
// Remove a transaction from the list.
//-----------------------------------------------------------------------------
int BS2TransactionManager::remove(BS2TransactionInfo * ti)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::remove");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    ti->m_tm = NULL;
    this->m_transaction_list.remove(ti);
#if 0
    this->m_transinfo_freelist.add (ti);
#else
    delete ti;
#endif
    return 0;
}

//-----------------------------------------------------------------------------
// Remove on all table entries until there is no transaction left.
//-----------------------------------------------------------------------------
// Must be called with lock held.
void BS2TransactionManager::removeAll(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::removeAll");

    GUARD_BEE(ACE_Thread_Mutex, ace_mon, this->m_lock);

    BS2TransactionInfo * ti;
    while ((ti = this->m_transaction_list.delete_head()) != 0)
    {
        delete ti;
    }
}

//-----------------------------------------------------------------------------
int BS2TransactionManager::checkState(TransactionType type, UINT tid)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::checkState");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, BEE_ERROR);

    // Not calling from self, have to look it up from the list.
    BS2TransactionInfo * ptr = this->find(tid);
    if (ptr == NULL)
    {
        return BEE_SUCCESS;
    }
    TransactionType trans_type = (TransactionType)ptr->m_type;
    return (trans_type == type);
}

//-----------------------------------------------------------------------------
// Get group ids for a particular transaction id.
//-----------------------------------------------------------------------------
int BS2TransactionManager::getType(UINT tid, TransactionType &type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::getType");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    BS2TransactionInfo * ptr = this->find(tid);
    if (ptr != NULL)
    {
        type = (TransactionType)ptr->m_type;
    }
    else
    {
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Set group type for a particular transaction id.
//-----------------------------------------------------------------------------
int BS2TransactionManager::setType(UINT tid, TransactionType type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::setType");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    BS2TransactionInfo * ptr = this->find(tid);
    if (ptr != NULL)
    {
        ptr->m_type = type;
    }
    else
    {
        return -1;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Returns the number of transactions in an transaction type.
//-----------------------------------------------------------------------------
int BS2TransactionManager::numTransactionsInType(TransactionType type)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::numTransactionsInType");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    int trans_count = 0;
    TransactionInfoListIter iter(this->m_transaction_list);
    for ( ; ! iter.done(); iter.advance())
    {
        if (type == TRANSACTION_UNKNOWN || iter.next()->m_type == type)
        {
            trans_count++;
        }
    }
    return trans_count;
}


//-----------------------------------------------------------------------------
// Returns in transaction_list a list of transaction numbers in an type.
//-----------------------------------------------------------------------------
int BS2TransactionManager::transactionList(TransactionType type, UINT tid_list[],
                                           size_t n)
{
    TRACE_FUNCTION(TRL_LOW, "BS2TransactionManager::transactionList");
    GUARD_NOT_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, -1);

    size_t tid_count = 0;
    TransactionInfoListIter iter(this->m_transaction_list);
    for ( ; ! iter.done(); iter.advance())
    {
        if (tid_count >= n)
        {
            break;
        }
        if (type == TRANSACTION_UNKNOWN || type == iter.next()->m_type)
        {
            tid_list[tid_count] = iter.next()->m_transid;
            tid_count++;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2TransactionManager::dump(void)
{
    // Cast away const-ness of this in order to use its non-const m_lock.
    GUARD_BEE(ACE_Thread_Mutex, ace_mon, this->m_lock);

    ACE_DEBUG((LM_DEBUG, ACE_BEGIN_DUMP, this));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_transaction_id = 0x%X"),
                        this->m_transaction_id));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_srcid = %d"), this->m_srcid));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\ncurrent count = %d"),
                         this->m_transaction_list.size()));
    for (TransactionInfoListIter iter(this->m_transaction_list); ! iter.done();
         iter.advance())
    {
        iter.next()->dump();
    }
    ACE_DEBUG((LM_DEBUG, ACE_END_DUMP));
}


