// $Id: BS2Float8.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Float8.h
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
#include "BS2Float8.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Float8::BS2Float8(BS2Float8 const& rhs): BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Float8::BS2Float8");
    BS2Assert(rhs.m_t == ATOM_FLOAT8);
    this->copy((BS2Atom&)rhs);
}

//-----------------------------------------------------------------------------
BS2Float8::BS2Float8(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Float8::BS2Float8");
    if (len >= sizeof(double))
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
const BS2Float8& BS2Float8::operator=(const BS2Float8& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::operator=");
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Float8& BS2Float8::operator=(const double data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::operator=");
    BS2Assert(m_t == ATOM_FLOAT8);
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Float8::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::set");
    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv((double)0);      // set type, qty and size
    if (itemHeader.dataLength() == sizeof(double))
    {
        buf >> m._do;
    }
    else
        m_q = 0;     // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Float8::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::setStreamData");
    initv((double)0);      // set type, qty and size
    BYTE * castPtr = (BYTE *)&m._do;

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
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Float8::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::get");

    int len = (m_q > 0) ? sizeof(double) : 0;
    BS2ItemHeader itemHeader(ATOM_FLOAT8, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._do;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Float8::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::getStreamData");
    if (! haveData())
        return;

    BYTE * castPtr = (BYTE *)&m._do;

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
// factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Float8::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::factory");
    if (len > sizeof(double))
    {
        BS2Float8Array * clone = new BS2Float8Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2Float8 * clone = new BS2Float8(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Float8::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::replicate");

    BS2Float8 * replica = new BS2Float8;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Float8& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Float8&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Float8& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Float8&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Float8::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::print");

    if (haveData())
        interp->printf(interp->print_xml() ? _TX(" %e ") : _TX(" %e") , m._do);
    else
        interp->printf(interp->print_xml() ? _TX(" ") : _TX(" {}"));
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Float8::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float8::dump");
    if (haveData())
        b_printf(_TX(" %e"), m._do);
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***
