// $Id: BS2List.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2List.cpp
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

#include "BS2value.h"
#include "BS2ItemHeader.h"
#include "BS2Item.h"
#include "BS2DeclAtoms.h"
#include "BS2Interpreter.h"
#include "BS2Stream.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2List::BS2List(BS2List const& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2List::BS2List");
    BS2Assert(rhs.m_t == ATOM_LIST);
}

//-----------------------------------------------------------------------------
BS2List::~BS2List()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2List::~BS2List");
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2List& BS2List::operator=(const BS2List& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::operator=");
    BS2Assert(rhs.m_t == ATOM_LIST);
    if (this == &rhs)
        return *this;
    this->BS2value::operator=((const BS2value&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
// Add member
//-----------------------------------------------------------------------------
void BS2List::add(const BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2List::add");
    BS2Assert(m_t == ATOM_LIST);
    BS2Atoms * atoms = reinterpret_cast<BS2Atoms *>(m._vec);
    if (m._vec == NULL)
    {
        atoms = new BS2Atoms;
        m._vec = (b_valvect *)atoms;
        m_q = 0;
    }
    atoms->push_back(const_cast<BS2Atom *>(atom));
    m_q++;
}

//-----------------------------------------------------------------------------
void BS2List::add(const BS2value& val)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2List::add");
    BS2Assert(m_t == ATOM_LIST);
    BS2Atoms * atoms = reinterpret_cast<BS2Atoms *>(m._vec);
    if (m._vec == NULL)
    {
        atoms = new BS2Atoms;
        m._vec = (b_valvect *)atoms;
        m_q = 0;
    }
    if (val.m_t == ATOM_LIST && ((val.m._vec != NULL) && val.m._vec->size() > 0))
    {
        b_valvect * src = val.m._vec;
        for (size_t i = 0; i < src->size(); i++)
        {
            BS2value * mbr = (BS2value *)((*src)[i]);
            atoms->push_back(BS2Atom::factory(*mbr));
        }
    }
    else
    {
        atoms->push_back(BS2Atom::factory(val));
    }
    m_q++;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2List::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::set");

    BS2ItemHeader itemHeader;
    m._vec->clear();   // release old atoms
    buf >> itemHeader;
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2List::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::setStreamData");
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2List::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::get");

    int length = m._vec->size();
    BS2ItemHeader itemHeader(ATOM_LIST, length);
    buf << itemHeader;

    if (length > 0)
    {
        BS2Atoms * atoms = reinterpret_cast<BS2Atoms *>(m._vec);
        BS2Atoms::const_iterator iter;
        for (iter = atoms->begin(); iter != atoms->end(); ++iter)
        {
            BS2Atom * atom = *iter;
            atom->get(buf);
        }
    }
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2List::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::getStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2List::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::factory");
    BS2value tmpval(data, len);
    BS2List * clone = new BS2List();
    clone->add(tmpval);
    return (BS2Atom *)clone;
}

//-----------------------------------------------------------------------------
BS2Atom * BS2List::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2List::replicate");

    BS2List * replica = new BS2List;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2List& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2List&)");

    BS2ItemHeader itemHeader;
    is >> itemHeader;
    atom.m_q = itemHeader.dataLength();
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2List& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2List&)");

    BS2ItemHeader itemHeader(atom.format(), atom.size());
    os << itemHeader;
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2List::print(BS2InterpBase * interp) const
{
    BS2Atoms * atoms = reinterpret_cast<BS2Atoms *>(m._vec);
    if (interp->print_xml())
    {
        if (atoms->size() > 0)
        {
            interp->printf(_TX(" <list> "));
            BS2Atoms::const_iterator iter;
            for (iter = atoms->begin(); iter != atoms->end(); ++iter)
            {
                BS2Atom * atom = reinterpret_cast<BS2Atom *>(*iter);
                atom->print(interp);
            }
            interp->printf(_TX(" </list> "));
        }
        else
        {
            interp->printf(_TX(" <list/> "));
        }
    }
    else
    {
        if (atoms->size() > 0)
        {
            interp->printf(_TX(" {"));
            BS2Atoms::const_iterator iter;
            for (iter = atoms->begin(); iter != atoms->end(); ++iter)
            {
                BS2Atom * atom = reinterpret_cast<BS2Atom *>(*iter);
                atom->print(interp);
            }
            interp->printf(_TX("} "));
        }
        else
        {
            interp->printf(_TX(" {} "));
        }
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2List::dump() const
{
    BS2Atoms * atoms = reinterpret_cast<BS2Atoms *>(m._vec);
    if (atoms->size() > 0)
    {
        BS2Atoms::const_iterator iter;
        for (iter = atoms->begin(); iter != atoms->end(); ++iter)
        {
            BS2Atom * atom = reinterpret_cast<BS2Atom *>(*iter);
            atom->dump();
        }
    }
}

//
// *** End of File ***
