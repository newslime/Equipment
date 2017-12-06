// $Id: BS2UInt1.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2UInt1.cpp
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
#include "BS2UInt1.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2UInt1::BS2UInt1(BS2UInt1 const& rhs): BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2UInt1::BS2UInt1");
    BS2Assert(rhs.m_t == ATOM_UINT1);
}

//-----------------------------------------------------------------------------
BS2UInt1::BS2UInt1(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2UInt1::BS2UInt1");
    if (len >= sizeof(BYTE))
    {
        initv(*data);
    }
    else
    {
        initNull();
    }
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2UInt1& BS2UInt1::operator=(const BS2UInt1& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2UInt1::operator=");
    BS2Assert(m_t == ATOM_UINT1 && rhs.m_t == ATOM_UINT1);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2UInt1& BS2UInt1::operator=(BYTE data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::operator=");
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2UInt1::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::set");

    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv((BYTE)0);      // set type, qty and size
    if (itemHeader.dataLength() == sizeof(BYTE))
    {
        buf >> m._uc;
    }
    else
        m_q = 0;     // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2UInt1::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::setStreamData");
    m._uc = *buf;
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2UInt1::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::get");

    int len = haveData() ? sizeof(BYTE) : 0;
    BS2ItemHeader itemHeader(ATOM_UINT1, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._uc;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2UInt1::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::getStreamData");
    if (! haveData())
        return;
    *(buf) = m._uc;
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2UInt1::factory( BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::factory");
    if (len > sizeof(BYTE))
    {
        BS2UInt1Array * clone = new BS2UInt1Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2UInt1 * clone = new BS2UInt1(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2UInt1::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::replicate");

    BS2UInt1 * replica = new BS2UInt1;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2UInt1& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2UInt1&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2UInt1& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2UInt1&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2UInt1::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::print");

    if (haveData())
        interp->printf(interp->print_xml() ? _TX(" %u ") : _TX(" %u"), m._uc);
    else
        interp->print(interp->print_xml() ? _TX(" ") : _TX(" {}"));
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2UInt1::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2UInt1::dump");
    if (haveData())
        b_printf(_TX(" %u"), m._uc);
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***
