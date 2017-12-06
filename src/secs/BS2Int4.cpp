// $Id: BS2Int4.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Int4.cpp
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

#include "BS2ItemHeader.h"
#include "BS2Int4.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Int4::BS2Int4(BS2Int4 const& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int4::BS2Int4");
    BS2Assert(rhs.m_t == ATOM_INT4);
}

//-----------------------------------------------------------------------------
BS2Int4::BS2Int4(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int4::BS2Int4");

    if (len >= sizeof(int))
    {
        this->setStreamData(data);
    }
    else
    {
        initNull();
    }
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Int4& BS2Int4::operator=(const BS2Int4& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::operator=");
    BS2Assert(m_t == ATOM_INT4 && rhs.m_t == ATOM_INT4);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Int4& BS2Int4::operator=(int data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::operator=");
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Int4::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::set");

    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv((int)0);      // set type, qty and size
    if (itemHeader.dataLength() == sizeof(int))
    {
        buf >> m._in;
    }
    else
        m_q = 0;     // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Int4::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::setStreamData");

    initv((int)((*buf << 24) + (*(buf + 1) << 16) + (*(buf + 2) << 8) +
                 *(buf + 3)));
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Int4::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::get");
    BS2Assert(m_t == ATOM_INT4);

    int len = haveData() ? size() : 0;
    BS2ItemHeader itemHeader(ATOM_INT4, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._in;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Int4::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::getStreamData");
    if (! haveData())
        return;

    *(buf + 0) = (m._in >> 24) & 0xFF;
    *(buf + 1) = (m._in >> 16) & 0xFF;
    *(buf + 2) = (m._in >> 8) & 0xFF;
    *(buf + 3) = m._in & 0xFF;

}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Int4::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::factory");
    if (len > sizeof(int))
    {
        BS2Int4Array * clone = new BS2Int4Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2Int4 * clone = new BS2Int4(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Int4::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::replicate");

    BS2Int4 * replica = new BS2Int4;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Int4& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Int4&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Int4& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Int4&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Int4::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::print");

    if (haveData())
        interp->printf(interp->print_xml() ? _TX(" %d ") : _TX(" %d"), m._in);
    else
        interp->printf(interp->print_xml() ? _TX(" ") : _TX(" {}"));
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Int4::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int4::dump");
    if (haveData())
        b_printf(_TX(" %d"), m._in);
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***
