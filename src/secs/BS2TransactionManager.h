// $Id: BS2TransactionManager.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TransactionManager.h
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

#ifndef BS2TRANSACTIONMANAGER_H
#define BS2TRANSACTIONMANAGER_H

#include "beesecs.h"
#include "BS2TransactionInfo.h"

#if !defined (ACE_DEFAULT_THREAD_MANAGER_LOCK)
#define ACE_DEFAULT_THREAD_MANAGER_LOCK    ACE_SYNCH_MUTEX
#endif /* ACE_DEFAULT_THREAD_MANAGER_LOCK */

#define MAKE_TRANSACTION_IDENTIFIER(s,t)  (((s) << 16) | ((t) & 0xFFFF))
#define GET_TRANSACTION_NUMBER(n)         ((n) & 0xFFFF)
#define GET_SOURCE_NUMBER(n)              (((n) >> 16) & 0xFFFF)

// Forward declarations.
class BS2IStream;
class BS2Message;
class BS2MessageInfo;

typedef ACE_Double_Linked_List<BS2TransactionInfo> TransactionInfoList;
typedef ACE_Double_Linked_List_Iterator<BS2TransactionInfo>
        TransactionInfoListIter;

//-----------------------------------------------------------------------------
//
// Manages a pool of BS2TransactionInfo.
//
//-----------------------------------------------------------------------------
class BEE_Export BS2TransactionManager
{	

friend class BS2TransactionInfo;

public:
    BS2TransactionManager(UINT srcid = 1) 
	:m_transaction_id(MAKE_TRANSACTION_IDENTIFIER(srcid, 0)),
     m_srcid(srcid) 
	{ 
	}
    virtual ~BS2TransactionManager(void) 
	{ 
		this->close(); 
	}

    // Get pointer to <BS2TransactionManager>.
    static BS2TransactionManager * instance(void);

    // Set pointer to <BS2TransactionManager> and return existing pointer.
    static BS2TransactionManager * instance(BS2TransactionManager *);

    // Delete the dynamically allocated Singleton
    static void close_singleton(void);

    int  open(size_t size = 0) { return 0; }
    int  close();
    void sourceId(int srcid)   { m_srcid = srcid; }

    // Return a pointer to the thread's Thread_Descriptor, <NULL> if fail.
    BS2TransactionInfo * buffer(BS2BlockHeader * header,
                                TransactionType type = TRANSACTION_UNKNOWN);
    BS2TransactionInfo * buffer(BS2OStream * stmbuf,
                                TransactionType type = TRANSACTION_UNKNOWN);

    // Returns the number of transactions in an <BS2TransactionType>.
    int numTransactionsInType(TransactionType type = TRANSACTION_UNKNOWN);

    // Returns in <tid_list> a list of up to <n> transaction ids in
    // an <TransactionType>.  The caller must allocate memory for <tid_list>.
    int transactionList(TransactionType type, UINT tid_list[], size_t n);

    // Insert a thread in the table (checks for duplicates).
    BS2TransactionInfo * insert(TransactionType type, BS2OStream * stmbuf,
                                BS2MessageInfo * rtinfo);

    // Locate the index of the table slot occupied by <tid>.  Returns
    // NULL if <tid> is not in the table doesn't contain <tid>.
    BS2TransactionInfo * find(UINT tid,
                              TransactionType type = TRANSACTION_UNKNOWN);

    // Get the header of transaction, and remove the transaction.
    BS2MessageInfo * info(BS2TransactionInfo * ti, int result, BS2Message * msg);

    int  remove(BS2TransactionInfo * ti);
    void removeAll(void);       // Remove all transactions from the table.
    UINT getTransactionId();
    void dump(void);

protected:
    int insert(UINT tid, TransactionType = TRANSACTION_UNKNOWN,
               BS2TransactionInfo * ti = NULL);

    // Append a transaction in the table.
    int append(UINT tid, TransactionType type, BS2TransactionInfo * ti = NULL);

    int checkState(TransactionType type, UINT tid);
    UINT transaction_id();      // get and increment transaction identifier.
    int  getType(UINT tid, TransactionType &type);
    int  setType(UINT tid, TransactionType type);

    // = ACE_Thread_Mutex and condition variable for synchronizing termination.
    ACE_Thread_Mutex m_lock;

    TransactionInfoList m_transaction_list;

private:
    UINT m_transaction_id;
    UINT m_srcid;

    ACE_Locked_Free_List<BS2TransactionInfo, ACE_SYNCH_MUTEX> m_transinfo_freelist;

    // Pointer to a process-wide <BS2TransactionManager>.
    static BS2TransactionManager *_trans_mgr;

    // Must delete the <_trans_mgr> if non-0.
    static int _delete_trans_mgr;
};



#endif /* _BS2TRANSACTIONMANAGER_H */
