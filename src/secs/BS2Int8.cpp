// $Id: BS2Int8.cpp,v 1.7 2004/06/27 08:01:56 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Int8.cpp
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
#include "BS2Int8.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Int8::BS2Int8(BS2Int8 const& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int8::BS2Int8");
    BS2Assert(rhs.m_t == ATOM_INT8);
}

//-----------------------------------------------------------------------------
BS2Int8::BS2Int8(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Int8::BS2Int8");
    if (len >= sizeof(LONGLONG))
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
const BS2Int8& BS2Int8::operator=(const BS2Int8& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::operator=");
    BS2Assert(m_t == ATOM_INT8 && rhs.m_t == ATOM_INT8);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Int8& BS2Int8::operator=(LONGLONG data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::operator=");
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Int8::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::set");

    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv((LONGLONG)0);      // set type, qty and size
    if (itemHeader.dataLength() == sizeof(LONGLONG))
    {
        buf >> m._llo;
    }
    else
        m_q = 0;        // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Int8::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::setStreamData");

    initv((LONGLONG)0);
    BYTE * castPtr = (BYTE *)&m._llo;
#if ENDIAN == LITTLE_ENDIAN
    *(castPtr + 0) = *(buf + 7);
    *(castPtr + 1) = *(buf + 6);
    *(castPtr + 2) = *(buf + 5);
    *(castPtr + 3) = *(buf + 4);
    *(castPtr + 4) = *(buf + 3);
    *(castPtr + 5) = *(buf + 2);
    *(castPtr + 6) = *(buf + 1);
    *(castPtr + 7) = *(buf + 0);
#else
    *(castPtr + 0) = *(buf + 0);
    *(castPtr + 1) = *(buf + 1);
    *(castPtr + 2) = *(buf + 2);
    *(castPtr + 3) = *(buf + 3);
    *(castPtr + 4) = *(buf + 4);
    *(castPtr + 5) = *(buf + 5);
    *(castPtr + 6) = *(buf + 6);
    *(castPtr + 7) = *(buf + 7);
#endif
    m_q = 1;     // exist data
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Int8::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::get");
    BS2Assert(m_t == ATOM_INT8);

    int len = haveData() ? size() : 0;
    BS2ItemHeader itemHeader(ATOM_INT8, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._llo;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Int8::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::getStreamData");
    BYTE * castPtr = (BYTE *)&m._llo;

    if (! haveData())
        return;

#if ENDIAN == LITTLE_ENDIAN
    *(buf + 7) = *(castPtr + 0);
    *(buf + 6) = *(castPtr + 1);
    *(buf + 5) = *(castPtr + 2);
    *(buf + 4) = *(castPtr + 3);
    *(buf + 3) = *(castPtr + 4);
    *(buf + 2) = *(castPtr + 5);
    *(buf + 1) = *(castPtr + 6);
    *(buf + 0) = *(castPtr + 7);
#else
    *(buf + 0) = *(castPtr + 0);
    *(buf + 1) = *(castPtr + 1);
    *(buf + 2) = *(castPtr + 2);
    *(buf + 3) = *(castPtr + 3);
    *(buf + 4) = *(castPtr + 4);
    *(buf + 5) = *(castPtr + 5);
    *(buf + 6) = *(castPtr + 6);
    *(buf + 7) = *(castPtr + 7);
#endif
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Int8::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::factory");
    if (len > sizeof(LONGLONG))
    {
        BS2Int8Array * clone = new BS2Int8Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2Int8 * clone = new BS2Int8(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Int8::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::replicate");

    BS2Int8 * replica = new BS2Int8;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Int8& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Int8&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Int8& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Int8&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Int8::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::print");

    if (haveData())
    {
#ifdef _MSC_VER
        interp->printf(interp->print_xml() ? _TX(" %I64d ") : _TX(" %I64d"), m._llo);
#else
        interp->printf(interp->print_xml() ? _TX(" %lld ") : _TX(" %lld"), m._llo);
#endif
    }
    else
        interp->printf(interp->print_xml() ? _TX(" ") : _TX(" {}"));

}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Int8::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Int8::dump");
    if (haveData())
    {
#ifdef _MSC_VER
        b_printf(_TX(" %I64d"), m._llo);
#else
        b_printf(_TX(" %lld"), m._llo);
#endif
    }
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***
