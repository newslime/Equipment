// $Id: BS2ListItem.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ListItem.cpp
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

#include "BS2ListItem.h"
#include "BS2Interpreter.h"

static BS2Itemtypes _listType;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ListItem::BS2ListItem() : BS2Item(&_listType)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ListItem::BS2ListItem");
    m_atom = NULL;
}

//-----------------------------------------------------------------------------
BS2ListItem::BS2ListItem(BS2Atom * atom) : BS2Item(&_listType, atom)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ListItem::BS2ListItem");
}

//-----------------------------------------------------------------------------
BS2ListItem::BS2ListItem(const BS2ListItem& rhs): BS2Item(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ListItem::BS2ListItem");
    // if (rhs.atom())
    //    m_atom = rhs.atom()->replicate();
    m_itemType = &_listType;
}

//-----------------------------------------------------------------------------
BS2ListItem::~BS2ListItem()
{
    if (m_atom != NULL)
        delete m_atom;

    if (m_items.size() > 0)
    {
        BS2Items::iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            BS2Item * item = *iter;
            delete item;
        }
    }
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2ListItem& BS2ListItem::operator=(const BS2ListItem& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::operator=");
    if (this == &rhs)
        return *this;

    if (rhs.items().size() > 0)
    {
        BS2Items::const_iterator iter;
        for (iter = rhs.items().begin(); iter != rhs.items().end(); ++iter)
        {
            BS2Item * item = *iter;
            BS2Item * replica = item->replicate();
            m_items.push_back(replica);
        }
    }
    return *this;
}

//-----------------------------------------------------------------------------
// Clone
//-----------------------------------------------------------------------------
BS2Item * BS2ListItem::clone() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::operator=");
    BS2ListItem * clone = new BS2ListItem(*this);
    if (items().size() > 0)
    {
        BS2Items::const_iterator iter;
        for (iter = items().begin(); iter != items().end(); ++iter)
        {
            BS2Item * item = *iter;
            BS2Item * cloneItem = item->clone();
            clone->m_items.push_back(cloneItem);
        }
    }
    return clone;
}


//-----------------------------------------------------------------------------
// Property
//-----------------------------------------------------------------------------
int BS2ListItem::itemCount() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::itemCount");
    int count = m_items.size();
    BS2Items::const_iterator iter;
    for (iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        BS2Item * item = *iter;
        if (item == NULL)
            throw invalid_argument(_TX("BS2ListItem::itemCount()"));
        if (item->isList())
        {
            count += ((BS2ListItem *)item)->itemCount();
        }
    }

    return count + 1;   // add self num.
}

//-----------------------------------------------------------------------------
int BS2ListItem::size() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::size");
    int size = 0;
    BS2Items::const_iterator iter;
    for (iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        BS2Item * item = *iter;
        if (item->isList())
        {
            size += ((BS2ListItem *)item)->size();
        }
        else
        {
            size += item->size();
        }
    }

    return size;
}

//-----------------------------------------------------------------------------
// Find member item
//-----------------------------------------------------------------------------
BS2Item * BS2ListItem::find(BCHAR * itemName) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::find");
    BS2Items::const_iterator iter;
    for (iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        BS2Item * item = *iter;
        if (item == NULL)
            throw invalid_argument(_TX("BS2ListItem::find()"));
        if (item->name() == itemName)
        {
            return item;
        }
    }
    return NULL;             // not found.
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2ListItem::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::print");

    if (interp->print_xml())
    {
        if (m_items.size() > 0)
        {
            BS2Items::const_iterator iter;
            interp->printf(_TX(" <List> "));
            for (iter = m_items.begin(); iter != m_items.end(); ++iter)
            {
                BS2Item * item = *iter;
                item->print(interp);
            }
            interp->printf(_TX(" </List> "));
        }
        else
        {
            interp->printf(_TX(" <List/> "));
        }
    }
    else
    {
        if (m_items.size() > 0)
        {
            BS2Items::const_iterator iter;
            interp->printf(_TX(" {"));
            for (iter = m_items.begin(); iter != m_items.end(); ++iter)
            {
                BS2Item * item = *iter;
                item->print(interp);
            }
            interp->printf(_TX("} "));
        }
        else
        {
            interp->printf(_TX(" { } "));
        }
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2ListItem::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ListItem::dump");
    if (m_items.size() > 0)
    {
        BS2Items::const_iterator iter;
        b_printf(_TX(" {"));
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            BS2Item * item = *iter;
            item->dump();
        }
        b_printf(_TX("} "));
    }
    else
    {
        b_printf(_TX(" { } "));
    }
}


//
// *** End of File ***
