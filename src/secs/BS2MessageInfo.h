// $Id: BS2MessageInfo.h,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2MessageInfo.h
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

#ifndef BS2MESSAGEINFO_H
#define BS2MESSAGEINFO_H

#include "beesecs.h"

class BS2Message;

enum                                    // code of result
{
    BS2RET_NORMAL = 0,
    BS2RET_ERROR = -1,
    BS2RET_ABORT = -2,
    BS2RET_TIMEOUT = -3,                // send S9F9
    BS2RET_TIMEOUT_MESSAGE = -4,        // receive S9F9,F13
    BS2RET_ILLEGAL_MESSAGE = -5,
};

//-----------------------------------------------------------------------------
//
// Result information
//
//-----------------------------------------------------------------------------
class BEE_Export BS2MessageInfo
{
public:
    BS2MessageInfo() 
	: m_status(0), 
	  m_result(BS2RET_NORMAL), 
	  m_tid(0),
      m_del(false), 
	  m_msg(NULL), 
	  m_userptr(NULL)
    {
        TRACE_FUNCTION(TRL_CONSTRUCT, "BS2MessageInfo::BS2MessageInfo()");
    }
    BS2MessageInfo(int result, BS2Message * msg);
    /*~BS2MessageInfo()
    {
        TRACE_FUNCTION(TRL_CONSTRUCT, "BS2MessageInfo::~BS2MessageInfo");
    }*/
	~BS2MessageInfo();

    BS2MessageInfo& operator=(const BS2MessageInfo& rhs);
    void moveto(BS2MessageInfo* dst);
    int  getResult() const     { return m_result; }
    void setResult(int result) { m_result = result; }
    UINT getTransactionID() const   
	{ 
		return m_tid; 
	}
    void setTransactionID(UINT tid) 
	{ 
		m_tid = tid; 
	}
    void * getUserPtr() const   { return m_userptr; }
    void setUserPtr(void * ptr) { m_userptr = ptr; }
    int  getStatus() const      { return m_status; }
    void setStatus(int status)  { m_status = status; }

    // A caller cause is to do a release of BS2Message. !!
    BS2Message*  message()			{ return m_msg; }
    BS2Message*  getMessage()		{ return m_msg; }
    BS2Message*  getMessageAndClear();
	/*BS2Message * getMessageAndClear() 
	{ 
		return remove(); 
	}
	BS2Message * remove()
	{
		BS2Message* result = m_msg; 
		m_msg = NULL; 
		return result;
    }*/
    void setMessage(BS2Message * msg) { m_msg = msg; }
    bool canDelete() const 
	{ 
		return m_del; 
	}
    void setDelete()       
	{ 
		m_del = true; 
	}

    void dump() const;

protected:
    void copy(const BS2MessageInfo& rhs);

private:
    int  m_status;
    int  m_result;               // for BS2Driver
    UINT m_tid;                  // for BS2Driver
    bool m_del;                  // for BS2Driver
    BS2Message * m_msg;          // for BS2Driver (not used)
    void * m_userptr;
};


#endif  /* BS2MESSAGEINFO_H */

