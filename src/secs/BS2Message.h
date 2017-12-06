// $Id: BS2Message.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Message.h
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

#ifndef BS2MESSAGE_H
#define BS2MESSAGE_H

#include "BS2MessageType.h"
#include "BS2ListItem.h"
#include "BS2Traverser.h"

//
// Forward declarations
class BS2Message;
class BS2List;
class BS2IStream;
struct BS2BlockHeader;
typedef list<BS2Item *> BS2ItemList;
typedef list<BS2Atom *> BS2AtomList;

typedef int (*ClientProcedure)(void * clientData, BS2Item * item);
typedef list<BS2Message * > BS2MessageQueue;

//-----------------------------------------------------------------------------
// SECS Message class
//-----------------------------------------------------------------------------
class BEE_Export BS2Message
{
    friend class BS2Socket;
    friend class BS2MessageType;
    friend class BS2MessageDictionary;

public:
    virtual ~BS2Message();

	int ListSize(){ return m_items.size(); }
    const string& name() const     { return m_msgtype->name(); }
    const BCHAR * charName() const { return m_msgtype->charName(); }
    const string& manager() const { return m_msgtype->manager(); }
    UINT  sf() const           { return SFCODE(m_streamNum,m_functionNum); }
    int   direction() const    { return m_msgtype->dir(); }
    int   multi() const        { return m_msgtype->multi(); }
    bool  multiBlock() const   { return (m_msgtype->multi() == MULTI_BLOCK_MESSAGE); }
    UINT  inqNum() const       { return m_msgtype->inqNum(); }
    bool  isPrimary() const    { return m_msgtype->isPrimary(); }
    bool  isSecondary() const  { return m_msgtype->isSecondary(); }
    bool  isGrant() const      { return m_msgtype->isGrant(); }
    bool  reply() const        { return m_wbit; }
    int   blockType() const;
    int   itemCount() const    { return m_itemCount; }
    int   dataSize() const     { return m_dataSize; }
    bool  isComplete() const   { return (m_complete == 1); }
    bool  spooled() const      { return (m_spooled != 0) ? true : false; }
    void  spooled(bool flag)   { m_spooled = (flag) ? 1 : 0; }
    UINT  transNum() const
	{ 
		return m_transactionNum; 
	}
    void  transNum(UINT tn)
	{ 
		m_transactionNum = tn; 
	}
    void  deviceNum(int num)   { m_deviceNum = num; }
    void  wait(bool tf)        { m_wbit = tf; }
    BS2ItemList& items() const { return (BS2ItemList&)m_items; }
    //const BS2MessageType * messageType() const { return m_msgtype; }
	BS2MessageType* messageType() { return m_msgtype; } //for delete items

    int   get(BS2BlockHeader * header) const;
    void  set(const BS2BlockHeader * header);
    bool  isExpect(const BS2BlockHeader& header) const;
    void  copy(const BS2ItemList& proto);
    void  add(BS2Item * item);
    void  add(BS2ListItem * item) { this->add((BS2Item *)item); }

    int   traverse(ClientProcedure proc, void * clientData);
    int   traverse(BS2Traverser * udata);
    BS2Item * getItem(const BCHAR * name);       // Get item by name
    BS2Atom * getAtom(const BCHAR * name);       // Get atom by item name
    int   getValue(const BCHAR * name, vector<b_value>& retval);
    int   getId(const BCHAR * name, vector<b_id>& retid);
    int   getGrant();
    int   atomToItem(BS2AtomList * atoms);

    virtual bool noise() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void dump() const;

    static BS2Message * factory(UINT sf);
    static BS2Message * response(const BS2Message * msg);
    static BS2Message * response(const BS2Message * msg, BYTE ack,
                                 const BCHAR * iname);
    static BS2Message * response(const BS2Message * msg, UINT ack,
                                 const BCHAR * iname);
    static BS2Message * makeInquiry(const BS2Message * msg);

	//add by slime
	void EraseAtoms_Recursive(BS2List* list_atom);
//
protected:
    BS2Message();
    BS2Message(UINT sf);
    BS2Message(int device, bool w, int stnum, int funcnum,
               int blocknum = 0, UINT trans = 0);
    void releaseAll();
    int  factory_base(BS2IStream * buf);
//
private:
    bool listElements(BS2Itemtypes * listStruct, BS2ListItem * listItem,
                      BS2List * listAtom);
    bool parseAttrIDAndData(BS2Itemtypes * listStruct,
                            BS2ListItem * listItem, BS2List * listAtom);
    bool parseAttrData(BS2Itemtypes * listStruct, BS2ListItem * listItem,
                       BS2List * listAtom);
    int  parseList(BS2ListItem * listStruct, BS2ListItem * listItem,
                   BS2List * listAtom);
	
//
protected:
    BS2MessageType * m_msgtype;
    int  m_dataSize;
    int  m_itemCount;
    int  m_complete;
    int  m_spooled;
    BS2ItemList m_items;
    bool m_wbit;           // Message header for i/o stream
    int  m_deviceNum;      //
    int  m_streamNum;      //
    int  m_functionNum;    //
    int  m_blockNum;       //
    UINT m_transactionNum; //
};

#endif /* BS2MESSAGE_H */
