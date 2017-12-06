// $Id: BS2ListItem.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ListItem.h
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

#ifndef BS2LISTITEM_H
#define BS2LISTITEM_H

#include "BS2Item.h"

#ifndef BS2ITEMS_DECLARE
#define BS2ITEMS_DECLARE
typedef list<BS2Item *>  BS2Items;
#endif

/* ------------------------------------------------------------------------- */
class BEE_Export BS2ListItem : public BS2Item
{
public:
    BS2ListItem();
    BS2ListItem(BS2Atom * atom);
    BS2ListItem(const BS2ListItem& rhs);
    virtual ~BS2ListItem();
    virtual BS2Item * clone() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void dump() const;

    const BS2ListItem& operator=(const BS2ListItem& rhs);

    BS2Items& add(const BS2Item * item) {
            m_items.push_back((BS2Item *)item);
            return (BS2Items&)m_items;
        }
    BS2Items& items() const { return (BS2Items&)m_items; }
    int  itemCount() const;
    int  size() const;
    void clearAtom() { m_atom = NULL; }
    BS2Item * find(BCHAR * itemName) const;

    int memberq() { return (int)m_items.size(); }

//
private:
    BS2Items m_items;
};

#endif /* BS2LISTITEM_H */
