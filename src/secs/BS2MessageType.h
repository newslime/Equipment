// $Id: BS2MessageType.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2MessageType.h
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

#ifndef BS2MESSAGETYPE_H
#define BS2MESSAGETYPE_H

#include "BS2ItemType.h"

class BS2List;
class BS2IStream;

#ifndef SFCODE
#define SFCODE(s,f)         (((s) << 8) | (f))
#define STREAMCODE(sf)      (((sf) >> 8) & 0xFF)
#define FUNCCODE(sf)        ((sf) & 0xFF)
#endif

enum
{
    HOST_TO_EQUIPMENT = 1,
    EQUIPMENT_TO_HOST = 2,
    PRIMARY_TO_SECONDARY = 4,
    SECONDARY_TO_PRIMARY = 8,
};
#define BOTH_DIR  (HOST_TO_EQUIPMENT | EQUIPMENT_TO_HOST)

enum
{
    SINGLE_BLOCK_MESSAGE = 0,
    MULTI_BLOCK_MESSAGE = 0xFFFF,
    INQUIRE_MESSAGE = 0x80000,
    GRANT_MESSAGE = 0xC0000,
};

enum
{
    REPLY_NONE = 0,
    REPLY_REQUIRED = 1,
    REPLY_OPTION = 2,
};

//
// Forward declarations
class BS2Message;
class BS2Traverser;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2MessageType
{
friend class BS2Message;
//
public:
    BS2MessageType(UINT sf, int dir = BOTH_DIR, bool wait = true,
                   const string& mngr = NULL,
                   UINT multi = SINGLE_BLOCK_MESSAGE);
    virtual ~BS2MessageType() {}

    BS2Message * factory(const BS2IStream * buf = NULL) const;

    const string& name() const     { return m_name; }
    const BCHAR * charName() const { return m_name.c_str(); }
    const string& manager() const   { return m_mngrName; }
    void  manager(const string& mn) { m_mngrName = mn; }
    int   dir()   const        { return m_dir; }
    UINT  sfnum() const        { return m_sfcode; }
    bool  wait() const         { return m_wait; }
    bool  isPrimary() const    { return (m_sfcode & 1); }
    bool  isSecondary() const  { return ((m_sfcode & 1) == 0); }
    void  setGrant()           { m_multi = GRANT_MESSAGE; }
    bool  isGrant() const      { return (m_multi == GRANT_MESSAGE); }
    bool  isInquire() const    { return (m_multi == INQUIRE_MESSAGE); }
    int   multi() const        { return m_multi; }
    void  inqNum(UINT inq)     { m_inqNum = inq; }
    UINT  inqNum() const       { return m_inqNum; }
    void  add(BS2ItemType * itemType);
    void  add(BS2Itemtypes * items);

	void  Erase_Recursive(BS2ItemType* item);
	void  EraseList();

    BS2Itemtypes& body()       { return m_itemMember; }
    void  print(BS2InterpBase * interp) const;
    void  dump() const;

//
protected:
    UINT m_sfcode;
    int  m_dir;
    bool m_wait;
    string m_name;
    string m_mngrName;
    BS2Itemtypes m_itemMember;
    UINT m_inqNum;
    int  m_multi;
};


#endif /* BS2MESSAGETYPE_H */
