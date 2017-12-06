// $Id: BS2BlockHeader.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2BlockHeader.h
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

#ifndef BS2BLOCKHEADER_H
#define BS2BLOCKHEADER_H

#include "beesecs.h"

//-----------------------------------------------------------------------------
enum HSMS_SType
{
    STYPE_DATA_MSG = 0,
    STYPE_SELECT_REQ = 1,
    STYPE_SELECT_RSP = 2,
    STYPE_DESELECT_REQ = 3,
    STYPE_DESELECT_RSP = 4,
    STYPE_LINKTEST_REQ = 5,
    STYPE_LINKTEST_RSP = 6,
    STYPE_REJECT_REQ = 7,
    STYPE_SEPARATE_REQ = 9,
};

enum HSMS_SelectStatus
{
    HSMS_SELECT_OK = 0,
    HSMS_SELECT_ACTIVE = 1,
    HSMS_SELECT_NOT_READY = 2,
    HSMS_SELECT_CONNECTED = 3,
};

enum HSMS_DeselectStatus
{
    HSMS_DESELECT_OK = 0,
    HSMS_DESELECT_NOTCONNECT = 1,
    HSMS_DESELECT_ACTIVE = 2,
};

enum HSMS_RejectStatus
{
    HSMS_NOT_SUPPORT_STYPE = 1,
    HSMS_NOT_SUPPORT_PTYPE = 2,
    HSMS_NOT_OPEN_TRANSACTION = 3,
    HSMS_NOT_SELECTED_ENTITY = 4,
};

//-----------------------------------------------------------------------------
// BLOCK HEAER
//-----------------------------------------------------------------------------

#if _MSC_VER >= 1100
#pragma pack(push)
#endif

struct BS2BlockHeader
{
#if ENDIAN == LITTLE_ENDIAN
    BYTE m_hiDevId;             // reverse bit(7): 0 = H -> E, 1 = E -> H
    BYTE m_loDevId;
    BYTE m_hiMsgId: 7;
    BYTE m_w: 1;                // wait bit (1 = wait response)
    BYTE m_loMsgId;
    BYTE m_hiBlockNum;
    BYTE m_loBlockNum;
    BYTE m_system[4];           // 0-1: source id, 2-3: transaction id
#else
    BYTE m_hiDevId;             // reverse bit(7): 0 = H -> E, 1 = E -> H
    BYTE m_loDevId;
    BYTE m_w: 1;                // wait bit (1 = wait response)
    BYTE m_hiMsgId: 7;
    BYTE m_loMsgId;
    BYTE m_hiBlockNum;
    BYTE m_loBlockNum;
    BYTE m_system[4];           // 0-1: source id, 2-3: transaction id
#endif

    BS2BlockHeader() { memset(this, 0, sizeof(BS2BlockHeader)); }

    void setDeviceId(int devid) {
            m_hiDevId = ((devid >> 8) & 0x7F) + (m_hiDevId & 0x80);
            m_loDevId = devid & 0xFF;
        }
    int  getDeviceId() const {
            return (((m_hiDevId & 0x7F) << 8) + (m_loDevId & 0xFF)); }
    void setToHostBit()   { m_hiDevId |= 0x80; }
    void resetToHostBit() { m_hiDevId &= 0x7F; }
    int  getToHostBit()   { return m_hiDevId & 0x80; }
    void setSessionId(int sessionid) 
	{
		m_hiDevId = (sessionid >> 8) & 0xFF;
		m_loDevId = sessionid & 0xFF;
    }
    int  getSessionId() const 
	{
		return ((m_hiDevId << 8) + (m_loDevId & 0xFF)); 
	}
    void setStreamNum(int num)   
	{ 
		m_hiMsgId = num & 0x7F; 
	}
    void setFunctionNum(int num) 
	{ 
		m_loMsgId = num; 
	}
    void setBlockNum(int n) 
	{
		m_hiBlockNum = ((n >> 8) & 0x7F) + (m_hiBlockNum & 0x80);
		m_loBlockNum = n & 0xFF;
    }
    void setPType(int n)  { m_hiBlockNum = n & 0xFF; }
    void setSType(int n)  { m_loBlockNum = n & 0xFF; }
    void setEndBit()      { m_hiBlockNum |= 0x80; }
    void setSourceNum(int n) 
	{
		m_system[0] = (n >> 8) & 0xFF; m_system[1] = (n & 0xFF); 
	}
    void setTransactionNum(int n) 
	{
		m_system[2] = (n >> 8) & 0xFF; m_system[3] = (n & 0xFF); 
	}
    int  getBlockNum() const 
	{
		return (((m_hiBlockNum & 0x7F) << 8) + (m_loBlockNum & 0xFF)); 
	}
    int  getPType() const       
	{ 
		return m_hiBlockNum; 
	}
    int  getSType() const       
	{ 
		return m_loBlockNum; 
	}
    UINT getStreamNum() const   
	{ 
		return (m_hiMsgId & 0x7F); 
	}
    UINT getFunctionNum() const 
	{ 
		return m_loMsgId; 
	}
    UINT getStreamFunctionNum() const 
	{
		return ((getStreamNum() << 8) | m_loMsgId); 
	}
    int  getSourceNum() const 
	{
		return ((m_system[0] << 8) + (m_system[1] & 0xFF)); 
	}
    int  getTransactionNum() const 
	{
		return ((m_system[2] << 8) + (m_system[3] & 0xFF)); 
	}
    bool isPrimary() const   
	{ 
		return ((getFunctionNum() & 1) == 1); 
	}
    bool isSecondary() const 
	{ 
		return ((getFunctionNum() & 1) == 0); 
	}
    void wait(bool b)        { m_w = b; }
    bool wait() const        { return m_w; }
    void end(bool b) 
	{
		m_hiBlockNum = (b) ? m_hiBlockNum | 0x80 : m_hiBlockNum & 0x7F; 
	}
    bool end() const 
	{ 
		return ((m_hiBlockNum & 0x80) != 0); 
	}
    bool isEqual(const BS2BlockHeader& other) const 
	{
            BCHAR * self = (BCHAR *)this;
            BCHAR * optr = (BCHAR *)&other;
            for (size_t i = 0; i < sizeof(BS2BlockHeader); i++)
                if (*self++ != *optr++)
                    return false;
            return true;
	}
    bool isEqual(BS2BlockHeader * other) { return isEqual(*other); }
    bool isExpect(const BS2BlockHeader& other) const 
	{
            // if ((m_hiDevId & 0x80) == (other.m_hiDevId & 0x80))
            //    return false;
            if (getDeviceId() != other.getDeviceId())
                return false;
            if (other.isPrimary())
                return false;
            if (getStreamNum() != other.getStreamNum())
                return false;
            if ((getFunctionNum() + 1) != other.getFunctionNum())
                return false;
            // if (other.getBlockNum() > 1)
            //    return false;
            if ((getSourceNum() == other.getSourceNum()) &&
                (getTransactionNum() == other.getTransactionNum()))
                return true;
            return false;
        }
    bool isExpect(BS2BlockHeader * other) { return isExpect(*other); }
    BS2BlockHeader& operator= (const BS2BlockHeader& other) {
            if (this == &other)
                return *this;
            memcpy((BCHAR *)this, (BCHAR *)&other, sizeof(BS2BlockHeader));
            return *this;
        }
    bool isSession() const 
	{
            if ( (getStreamNum() == 0) 
				 &&
                 ((getBlockNum() >= 1 && getBlockNum() <= 7) || (getBlockNum() == 9)))
                return true;
            
			if (getBlockNum() == 7)
                return true;
            
			return false;
    }
    bool wantSessionRsp() const {
            if (getBlockNum() == 1 || getBlockNum() == 3 || getBlockNum() == 5)
                return true;
            return false;
        }
    void dump() const {
#ifdef ACE_ACE_H
            ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n*** secs message header ***\n")));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" reverse = %d,"), (this->m_hiDevId >> 7) & 1));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" device-id = %x,"), this->getDeviceId()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" wait = %d,"), this->m_w));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" stream = %d,"), this->getStreamNum()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" function = %d\n"), this->getFunctionNum()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" end-bit = %d,"), this->end()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" block = %d,"), this->getBlockNum()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" source = %d,"), this->getSourceNum()));
            ACE_DEBUG((LM_DEBUG, ACE_TEXT(" transaction = %d\n"), this->getTransactionNum()));
#endif
        }
#ifdef __GNUC__
} __attribute__((packed));
#else
};
#endif

//-----------------------------------------------------------------------------
struct BS2HSMSHeader
{
    BYTE m_length[4];
    BS2BlockHeader m_head;
};


#if _MSC_VER >= 1100
#pragma pack(pop)
#endif


#endif
