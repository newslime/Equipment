// $Id: BS2ItemType.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ItemType.h
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

#ifndef BS2ITEMTYPE_H
#define BS2ITEMTYPE_H

#include "BS2Atom.h"

class BS2InterpBase;
class BS2Item;

typedef map<int, std::string, std::less<int> > ErrorTextMap;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2ItemType
{
friend class BS2ItemDictionary;

public:
    enum
    {
        LIST_BIT    = 1,
        BINARY_BIT  = 2,
        BOOLEAN_BIT = 4,
        ASCII_BIT   = 0x10,
        JIS_BIT     = 0x20,
        UNICODE_BIT = 0x40,
        INT1_BIT    = 0x100,
        INT2_BIT    = 0x200,
        INT4_BIT    = 0x400,
        INT8_BIT    = 0x800,
        UINT1_BIT   = 0x1000,
        UINT2_BIT   = 0x2000,
        UINT4_BIT   = 0x4000,
        UINT8_BIT   = 0x8000,
        FLOAT4_BIT  = 0x40000,
        FLOAT8_BIT  = 0x80000,
        UNKNOWN_FORMAT = 0xFFFFFFFF,
    };

public:
    BS2ItemType(const string& name, UINT format, int defaultFormat);
    virtual ~BS2ItemType() {}

    virtual BS2Item * factory(BS2Atom * atom);
    virtual void print(BS2InterpBase * interp);
    virtual void dump() const;

    const string& itemName() const    { return m_name; }
    void  itemName(const string& nm)  { m_name = nm; }
    const string& formatNames() const { return m_formatNames; }
    UINT formats() const { return m_formats; }
    void formats(UINT format) { m_formats = format; }
    bool availFormat(int form) const;
    int  defaultFormat() const   { return m_defaultFormat; }
    void defaultFormat(int form) { m_defaultFormat = form; }
    bool isList() const   { return m_list; }
    bool isObject() const { return m_isObj; }
    void setObject(bool tf = true) 
	{ 
		m_isObj = tf; 
	}
    void add(int errcode, const std::string& errtext)
    {
        m_errtext.insert(std::make_pair(errcode, errtext));
    }
    std::string errtext(int errcode)
    {
        ErrorTextMap::iterator iter = m_errtext.find(errcode);
        if (iter == m_errtext.end())
        {
            return "Unknown error";
        }
        return iter->second;
    }

//
protected:
    BS2ItemType() : m_list(false) {}
    static int bitToNum(UINT bit);

protected:
    string m_name;          // item name
    UINT   m_formats;
    int    m_defaultFormat;
    string m_formatNames;
    bool   m_list;
    bool   m_isObj;
    ErrorTextMap m_errtext;
};

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Itemtypes : public BS2ItemType
{
public:
    BS2Itemtypes() : BS2ItemType(_TX("List"), (UINT)BS2ItemType::LIST_BIT, (int)ATOM_LIST)
	{
		m_list = true;
    }
    
	virtual ~BS2Itemtypes() 
	{
        // Can't delete itemType
        // for (size_t i = 0; i < m_items.size(); i++) {
        //     BS2ItemType * itemType = m_items[i];
        //     if (itemType != NULL) {
        //         delete itemType;
        //     }
        // }
    }
    
	virtual BS2Item * factory(BS2Atom * atom);

    void add(BS2ItemType * itemType) 
	{
		m_items.push_back(itemType);
    }
    
	size_t size() const 
	{ 
		return m_items.size(); 
	}
    
	BS2ItemType* at(int p) const 
	{
		return (m_items.size() > (size_t)p) ? m_items[p] : NULL;
    }

//protected:
    vector<BS2ItemType *> m_items;
};

#endif /* BS2ITEMTYPE_H */
