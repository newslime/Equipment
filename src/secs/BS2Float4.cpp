// $Id: BS2Float4.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Float4.cpp
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
#include "BS2Float4.h"
#include "BS2Stream.h"
#include "BS2Array.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Float4::BS2Float4(const BS2Float4& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Float4::BS2Float4");
    BS2Assert(rhs.m_t == ATOM_FLOAT4);
}

//-----------------------------------------------------------------------------
BS2Float4::BS2Float4(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Float4::BS2Float4");
    if (len >= sizeof(float))
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
const BS2Float4& BS2Float4::operator=(const BS2Float4& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::operator=");
    BS2Assert(m_t == ATOM_FLOAT4 && rhs.m_t == ATOM_FLOAT4);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Float4& BS2Float4::operator=(float data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::operator=");
    BS2Assert(m_t == ATOM_FLOAT4);
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Float4::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::set");
    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    initv(0.0);     // set type, qty and size
    if ((size_t)itemHeader.dataLength() >= sizeof(float))
    {
        buf >> m._fl;
    }
    else
        m_q = 0;     // data is nothing
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Float4::setStreamData(BYTE * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::setStreamData");
    initv(0.0);
    BYTE * castPtr = (BYTE *)&m._fl;
#if ENDIAN == LITTLE_ENDIAN
    *(castPtr + 0) = *(buf + 3);
    *(castPtr + 1) = *(buf + 2);
    *(castPtr + 2) = *(buf + 1);
    *(castPtr + 3) = *(buf + 0);
#else
    *(castPtr + 0) = *(buf + 0);
    *(castPtr + 1) = *(buf + 1);
    *(castPtr + 2) = *(buf + 2);
    *(castPtr + 3) = *(buf + 3);
#endif
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Float4::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::get");
    BS2Assert(m_t == ATOM_FLOAT4);

    int len = haveData() ? sizeof(float) : 0;
    BS2ItemHeader itemHeader(ATOM_FLOAT4, 1, len);
    buf << itemHeader;
    if (len > 0)
        buf << m._fl;
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Float4::getStreamData(BYTE * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::getStreamData");
    if (! haveData())
        return;

    BYTE * castPtr = (BYTE *)&m._fl;

#if ENDIAN == LITTLE_ENDIAN
    *(buf + 3) = *(castPtr + 0);
    *(buf + 2) = *(castPtr + 1);
    *(buf + 1) = *(castPtr + 2);
    *(buf + 0) = *(castPtr + 3);
#else
    *(buf + 0) = *(castPtr + 0);
    *(buf + 1) = *(castPtr + 1);
    *(buf + 2) = *(castPtr + 2);
    *(buf + 3) = *(castPtr + 3);
#endif
}

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Float4::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::factory");
    if (len > sizeof(float))
    {
        BS2Float4Array * clone = new BS2Float4Array(data, len);
        return (BS2Atom *)clone;
    }
    else
    {
        BS2Float4 * clone = new BS2Float4(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Float4::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::replicate");

    BS2Float4 * replica = new BS2Float4;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Float4& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Float4&)");

    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Float4& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Float4&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Float4::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::print");

    if (haveData())
        interp->printf(interp->print_xml() ? _TX(" %f ") : _TX(" %f"), m._fl);
    else
        interp->printf(interp->print_xml() ? _TX(" ") : _TX(" {}"));
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Float4::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Float4::dump");
    if (haveData())
        b_printf(_TX(" %f"), m._fl);
    else
        b_printf(_TX(" {}"));
}

//
// *** End of File ***

