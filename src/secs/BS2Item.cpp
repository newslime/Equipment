// $Id: BS2Item.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Item.cpp
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

#include "BS2Int8.h"
#include "BS2Int1.h"
#include "BS2Int2.h"
#include "BS2Int4.h"
#include "BS2Float4.h"
#include "BS2Float8.h"
#include "BS2UInt8.h"
#include "BS2UInt1.h"
#include "BS2UInt2.h"
#include "BS2UInt4.h"
#include "BS2Array.h"
#include "BS2Item.h"
#include "BS2Interpreter.h"
#include "BS2ItemDictionary.h"

#ifndef _MSC_VER
#define _tcscat strcat
#define _tcslen strlen
#endif

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ItemBase::BS2ItemBase(const BS2ItemType * itemtype, BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemBase::BS2ItemBase");
    m_itemType = const_cast<BS2ItemType *>(itemtype);
    m_atom = atom;
    if (m_itemType->isList() &&
        (atom != NULL && atom->isList()))
    {
        m_itemCode = ATOM_LIST;
    }
    else
    {
        m_itemCode = (atom) ? atom->format() : m_itemType->defaultFormat();
    }
}

//-----------------------------------------------------------------------------
BS2ItemBase::BS2ItemBase(const BS2ItemBase& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemBase::BS2ItemBase");
    m_itemType = rhs.m_itemType;
    m_atom = (rhs.m_atom != NULL) ? rhs.m_atom->replicate() : NULL;
    m_itemCode = rhs.m_itemCode;
}

//-----------------------------------------------------------------------------
BS2ItemBase::~BS2ItemBase()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemBase::~BS2ItemBase");
    if (m_atom != NULL)
    {
        delete m_atom;
    }
}

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Item::BS2Item(const BS2Item& rhs): BS2ItemBase((BS2ItemBase&)rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Item::BS2Item");
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Item& BS2Item::operator=(const BS2Item& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::operator=");
    if (this == &rhs)
        return *this;

    if (m_atom != NULL)
        delete m_atom;

    m_itemType = rhs.m_itemType;
    m_atom = (rhs.m_atom != NULL) ? rhs.m_atom->replicate() : NULL;
    m_itemCode = rhs.m_itemCode;
    return *this;
}

//-----------------------------------------------------------------------------
// Factory object (static)
//-----------------------------------------------------------------------------
BS2Item * BS2Item::factory(const BCHAR * name, b_id& id)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::factory");

    BS2id _id(id);
    BS2Atom * atom = _id.getAtom();
    return (BS2ItemDictionary::instance()->factory(string(name), atom));
}

//-----------------------------------------------------------------------------
BS2Item * BS2Item::factory(const BCHAR * name, b_value& val)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::factory");

    BS2value _value(val);
    BS2Atom * atom = _value.getAtom();
    return (BS2ItemDictionary::instance()->factory(string(name), atom));
}

//-----------------------------------------------------------------------------
BS2Item * BS2Item::factory(const BCHAR * name, BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::factory");

    return (BS2ItemDictionary::instance()->factory(string(name), atom));
}

//-----------------------------------------------------------------------------
BS2Item * BS2Item::factory(const BCHAR * name)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::factory");

    return (BS2ItemDictionary::instance()->factory(string(name)));
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Item * BS2Item::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::replicate");
    BS2Assert(m_itemType != NULL);

    BS2Item * item = new BS2Item(m_itemType);
    item->m_itemCode = m_itemCode;
    return item;
}

//-----------------------------------------------------------------------------
BS2Item * BS2Item::clone(BS2Atom * atom) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::clone");
    BS2Item * item = new BS2Item(m_itemType, atom);
    item->m_itemCode = m_itemCode;
    return item;
}

//-----------------------------------------------------------------------------
BS2Item * BS2Item::clone() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::clone");
    BS2Item * item = new BS2Item(*this);
    item->m_itemType = m_itemType;
    item->m_atom = (m_atom != NULL) ? m_atom->replicate() : NULL;
    item->m_itemCode = m_itemCode;
    return item;
}

//-----------------------------------------------------------------------------
// Add atom (make array data)
//-----------------------------------------------------------------------------
int BS2Item::add(BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Item::add");
    if (m_atom == NULL)
    {
        m_atom = atom;
        return 0;
    }

    if ((m_atom->format() & ~ATOM_ARRAY) != (atom->format() & ~ATOM_ARRAY))
    {
        return 0;
    }
    if (atom->isArray())
    {   // not support to add array, yet.
        return 0;
    }

    if (m_atom->isArray())
    {
         if (m_atom->isInt4Array())
         {
             BS2Int4Array * arrayp = (BS2Int4Array *)m_atom;
             BS2Int4 * atomptr = (BS2Int4 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isUInt4Array())
         {
             BS2UInt4Array * arrayp = (BS2UInt4Array *)m_atom;
             BS2UInt4 * atomptr = (BS2UInt4 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isUInt2Array())
         {
             BS2UInt2Array * arrayp = (BS2UInt2Array *)m_atom;
             BS2UInt2 * atomptr = (BS2UInt2 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isInt2Array())
         {
             BS2Int2Array * arrayp = (BS2Int2Array *)m_atom;
             BS2Int2 * atomptr = (BS2Int2 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isInt1Array())
         {
             BS2Int1Array * arrayp = (BS2Int1Array *)m_atom;
             BS2Int1 * atomptr = (BS2Int1 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isInt8Array())
         {
             BS2Int8Array * arrayp = (BS2Int8Array *)m_atom;
             BS2Int8 * atomptr = (BS2Int8 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isUInt1Array())
         {
             BS2UInt1Array * arrayp = (BS2UInt1Array *)m_atom;
             BS2UInt1 * atomptr = (BS2UInt1 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isUInt8Array())
         {
             BS2UInt8Array * arrayp = (BS2UInt8Array *)m_atom;
             BS2UInt8 * atomptr = (BS2UInt8 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isFloat4Array())
         {
             BS2Float4Array * arrayp = (BS2Float4Array *)m_atom;
             BS2Float4 * atomptr = (BS2Float4 *)atom;
             arrayp->add(atomptr->value());
         }
         else if (m_atom->isFloat8Array())
         {
             BS2Float8Array * arrayp = (BS2Float8Array *)m_atom;
             BS2Float8 * atomptr = (BS2Float8 *)atom;
             arrayp->add(atomptr->value());
         }
         else
             return 0;
    }
    else
    {
        size_t newSize = m_atom->size() + atom->size();
        BYTE * vbuf = new BYTE[newSize];
        m_atom->getStreamData(vbuf);
        atom->getStreamData(vbuf + m_atom->size());
        BS2Atom * arrayp = m_atom->factory(vbuf, newSize);
        delete [] vbuf;
        delete m_atom;
        m_atom = arrayp;
    }
    return 1;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Item::print(BS2InterpBase * interp) const
{
    if (interp->print_xml())
    {
        if (m_atom != NULL)
        {
            interp->printf(_TX("    <item name=\"%s\" type=\"%s\"> "),
                           charName(), m_atom->formatName());
            m_atom->print(interp);
        }
        else
        {
            interp->printf(_TX("    <item name=\"%s\" type=\"%s\"> "),
                           charName(), formatNames().c_str());
        }
        interp->printf(_TX("   </item>\n"));
    }
    else
    {
        if (interp->print_tag())
            interp->printf(_TX(" :%s:"), charName());

        if (m_atom != NULL)
            m_atom->print(interp);
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Item::dump() const
{
    b_printf(_TX(" :%s:"), charName());
    if (m_atom != NULL)
        m_atom->dump();
}


//
// *** End of File ***
