// $Id: BS2Item.h,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Item.h
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

#ifndef BS2ITEM_H
#define BS2ITEM_H

#include "BS2ItemType.h"

enum
{
    ITEM_UNKNOWN = 0,
    ITEM_ANY,
    ITEM_LIST,
};


class BS2Item;

//------------------------------------------------------------------------------
class BEE_Export BS2ItemBase
{
    friend class BS2ItemType;

public:
    virtual ~BS2ItemBase();
    virtual BS2Item * clone() const = 0;
    virtual BS2Item * clone(BS2Atom * atom) const = 0;
    virtual void print(BS2InterpBase * interp) const = 0;
    virtual void dump() const = 0;

    const string& name() const     { return (m_itemType->itemName()); };
    const BCHAR * charName() const { return (m_itemType->itemName().c_str()); };
    bool  availFormat(int form) const { return m_itemType->availFormat(form); }
    int   size() const   { return ((m_atom == NULL) ? 0 : (int)m_atom->size()); }
    bool  isList() const { return (m_itemCode == ATOM_LIST); }
    bool  isListItem() const { return m_itemType->isList(); }
    bool  isObject() const { return m_itemType->isObject(); }
    BS2Atom * atom() const { return m_atom; }
    const string& formatNames() const { return m_itemType->formatNames(); }
//
protected:
    BS2ItemBase(const BS2ItemBase& rhs);
    BS2ItemBase(const BS2ItemType * itemtype, BS2Atom * atom = NULL);
    void setAtom(BS2Atom * atom) { m_atom = atom; }

protected:
    BS2ItemType * m_itemType;
    int     m_itemCode;
    BS2Atom * m_atom;
};

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Item: public BS2ItemBase
{
    friend class BS2ItemDictionary;
    friend class BS2ItemType;
    friend class BS2ItemTypes;

protected:
    BS2Item(const BS2Item& rhs);

public:
    BS2Item(const BS2ItemType * itemType, BS2Atom * atom = NULL)
            : BS2ItemBase(itemType, atom) {
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Item::BS2Item");
        }
    virtual ~BS2Item() {
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Item::~BS2Item");
        }
    virtual BS2Item * clone() const;
    virtual BS2Item * clone(BS2Atom * atom) const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void dump() const;

    BS2Item const& operator=(BS2Item const& proto);
    BS2Item * replicate() const;
    int add(BS2Atom * atom);

    std::string errtext(int errcode)
    {
        return m_itemType->errtext(errcode);
    }

    static BS2Item * factory(const BCHAR * name);
    static BS2Item * factory(const BCHAR * name, BS2Atom * atom);
    static BS2Item * factory(const BCHAR * name, b_value& val);
    static BS2Item * factory(const BCHAR * name, b_id& id);

};


#endif /* BS2ITEM_H */
