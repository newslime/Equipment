// $Id: BS2TransactionInfo.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TransactionInfo.h
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

#ifndef BS2TRANSACTIONINFO_H
#define BS2TRANSACTIONINFO_H

#include "beesecs.h"
#include "BS2BlockHeader.h"

#if !defined (ACE_DEFAULT_THREAD_MANAGER_LOCK)
#define ACE_DEFAULT_THREAD_MANAGER_LOCK    ACE_SYNCH_MUTEX
#endif

#define MAKE_TRANSACTION_IDENTIFIER(s,t)  (((s) << 16) | ((t) & 0xFFFF))
#define GET_TRANSACTION_NUMBER(n)         ((n) & 0xFFFF)
#define GET_SOURCE_NUMBER(n)              (((n) >> 16) & 0xFFFF)

// Forward declarations.
class BS2IStream;
class BS2OStream;
class BS2TransactionManager;
class BS2MessageInfo;

//-----------------------------------------------------------------------------
enum TransactionType
{
    TRANSACTION_UNKNOWN,
    TRANSACTION_SEND_PRIMARY,
    TRANSACTION_RECV_PRIMARY,
    TRANSACTION_RECV_SECONDARY,
    TRANSACTION_SEND_SESSION,
};

//-----------------------------------------------------------------------------
//
//   Information for controlling transactions that run under the control
//   of the <BS2TransactionManager>.
//
//-----------------------------------------------------------------------------
class BEE_Export BS2TransactionInfo
{

friend class BS2TransactionManager;

public:
    // = Initialization method.
    BS2TransactionInfo(void);
    BS2TransactionInfo(int tid, BS2OStream * stmbuf = NULL,
                       BS2MessageInfo * rtinfo = NULL);
    ~BS2TransactionInfo(void);

    UINT self(void) { return this->m_transid; }   // Unique transaction id.
    void self(UINT transaction_id) { m_transid = transaction_id; }
    int  type(void) { return m_type; }            // Get transaction type
    void type(TransactionType type) { m_type = type; }
    int  timer_id(void)   { return m_timerid; }   // Get transaction timer id.
    void timer_id(int id) { m_timerid = id; }
    int  incBlockCount() {               // Increment block number.
            int bcount = m_header.getBlockNum() + 1;
            m_header.setBlockNum(bcount);
            return ((bcount > SECS1_MAX_BLOCK) ? BEE_ERROR: BEE_SUCCESS);
        }
    int  blockCount() const { return (m_header.getBlockNum()); }
    void blockCount(int n)  { m_header.setBlockNum(n); }
    BS2OStream * buffer() const { return m_sbuf; }
    BS2OStream * removeBuffer() 
	{
		BS2OStream* stmbuf = m_sbuf;
        m_sbuf = NULL;
        return stmbuf;
    }
    BS2OStream * exchangeBuffer(BS2OStream * newbuf) 
	{
        BS2OStream* stmbuf = m_sbuf;
        m_sbuf = newbuf;
		return stmbuf;
    }
    BS2MessageInfo * eventHeader() const { return m_evtinfo; }
    void restoreHeader();                             // buffer --> header
    void timeout(int msec) { m_timeout.msec(msec); }  // set timeout value;
    void timeout(ACE_Time_Value& tv) { m_timeout.set((timeval)tv); }
    const ACE_Time_Value& timeout() const { return m_timeout; }

    int operator== (const BS2TransactionInfo &rhs) const {
            if ((m_transid == rhs.m_transid) &&
                 m_header.isExpect((BS2BlockHeader&)rhs.m_header))
                return 1;
            return 0;
        }
    int operator!= (const BS2TransactionInfo &rhs) const {
            return (! (*this == rhs));
        }
    void dump(void) const;    // Dump the state of an object.

    // Set/get the <next_> pointer.  These are required by the ACE_Free_List.
    void set_next(BS2TransactionInfo * ti) { this->next_ = ti; }
    BS2TransactionInfo * get_next(void)    { return this->next_; }
    BS2TransactionManager * getTransactionManager() const { return this->m_tm; }

    // We need these pointers to maintain the double-linked list in a
    // trasaction managers.
    BS2TransactionInfo * next_;
    BS2TransactionInfo * prev_;

private:
    UINT m_transid;                    // Unique transaction ID.
    int  m_type;                       // transaction type.
    int  m_registered;                 // Keep track of registration status.
    int  m_timerid;                    // Transaction timer ID.

    // Pointer to an <BS2TransactionManager> or NULL
    BS2TransactionManager * m_tm;

    // Registration lock to prevent premature removal of trasaction.
    ACE_DEFAULT_THREAD_MANAGER_LOCK * m_sync;

    BS2OStream * m_sbuf;               // Point of SECS output stream buffer
    BS2MessageInfo * m_evtinfo;        // Return info to receive method.
    BS2BlockHeader m_header;           // The last sended or received header.
    ACE_Time_Value m_timeout;

};

#endif /* BS2TRANSACTIONINFO_H */











