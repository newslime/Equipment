// $Id: BS2Int2.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Int2.cpp
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
#include "BS2Int2.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Int2::BS2Int2(const BS2Int2& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int2::BS2Int2");
    BS2Assert(rhs.m_t == ATOM_INT2);
}

//-----------------------------------------------------------------------------
BS2Int2::BS2Int2(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int2::BS2Int2");
    if (len >= sizeof(short))
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
const BS2Int2& BS2Int2::operator=(const BS2Int2& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::operator=");
    BS2Assert(m_t == ATOM_INT2 && rhs.m_t == ATOM_INT2);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Int2& BS2Int2::operator=(short data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::operator=");
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Int2::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::set");

    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv((short)0);      // set type, qty and size
    if (itemHeader.dataLength() == sizeof(short))
    {
        buf >> m._sh;
    }
    else
        m_q = 0;     // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Int2::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::setStreamData");

    initv((short)((*buf << 8) + *(buf + 1)));
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Int2::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::get");
    BS2Assert(m_t == ATOM_INT2);

    int len = haveData() ? sizeof(short) : 0;
    BS2ItemHeader itemHeader(ATOM_INT2, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._sh;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Int2::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::getStreamData");
    if (! haveData())
        return;

    *(buf + 0) = (m._sh >> 8) & 0xFF;
    *(buf + 1) = m._sh & 0xFF;
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Int2::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::factory");
    if (len > sizeof(short))
    {
        BS2Int2Array * clone = new BS2Int2Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2Int2 * clone = new BS2Int2(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Int2::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::replicate");

    BS2Int2 * replica = new BS2Int2;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Int2& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Int2&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Int2& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Int2&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Int2::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::print");

    if (haveData())
        interp->printf(interp->print_xml() ? _TX(" %d ") : _TX(" %d"), m._sh);
    else
        interp->printf(interp->print_xml() ? _TX(" ") : _TX(" {}"));
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Int2::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int2::dump");
    if (haveData())
        b_printf(_TX(" %d"), m._sh);
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***
