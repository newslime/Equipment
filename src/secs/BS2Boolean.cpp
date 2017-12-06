// $Id: BS2Boolean.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Boolean.cpp
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
#include "BS2Boolean.h"
#include "BS2Array.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Boolean::BS2Boolean(const BS2Boolean& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Boolean::BS2Boolean");
    BS2Assert(rhs.m_t == ATOM_BOOLEAN);
}

//-----------------------------------------------------------------------------
BS2Boolean::BS2Boolean(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Boolean::BS2Boolean");
    m_t = ATOM_BOOLEAN; m_sz = sizeof(bool);
    if (len > 0)
    {
        int result = 0;
        for (size_t i = 0; i < len; i++)
        {
            result |= *(data + i);
        }
        m._bo = (result != 0) ? true : false;
        m_q = 1;
    }
    else
    {
        m_q = 0;
        m._bo = false;
    }
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Boolean& BS2Boolean::operator=(const BS2Boolean& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::operator=");
    BS2Assert(m_t == ATOM_BOOLEAN && rhs.m_t == ATOM_BOOLEAN);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Boolean& BS2Boolean::operator=(bool data)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::operator=");
    BS2Assert(m_t == ATOM_BOOLEAN);
    initv(data);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Boolean::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::set");

    BS2ItemHeader itemHeader;
    char datum;
    buf >> itemHeader;
    int len = itemHeader.dataLength();
    if (len > 0)
    {
        buf >> datum;
        *this = (datum) ? true : false;
    }
    else
    {
        m_q = 0;
    }
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Boolean::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::setStreamData");
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Boolean::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::get");

    if (m_q > 0)
    {
        BS2ItemHeader itemHeader(ATOM_BOOLEAN, 1, 1);
        buf << itemHeader;
        BCHAR datum = (m._bo) ? ~0 : 0;
        buf << datum;
    }
    else
    {
        BS2ItemHeader itemHeader(ATOM_BOOLEAN, 1, 0);
        buf << itemHeader;
    }
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Boolean::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::getStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Boolean::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::factory");
    if (len > sizeof(bool))
    {
        BS2BoolArray * arrayClone = new BS2BoolArray(data, len);
        return (BS2Atom *)arrayClone;
    }
    else
    {
        BS2Boolean * clone = new BS2Boolean(data, len);
        return (BS2Atom *)clone;
    }
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Boolean::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::replicate");

    BS2Boolean * replica = new BS2Boolean;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Boolean& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Boolean&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, const BS2Boolean& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Boolean&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Boolean::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::print");

    if (interp->print_xml())
    {
        interp->printf(_TX(" %s "), m._bo ? _TX("true") : _TX("false"));
    }
    else
    {
        interp->printf(_TX(" %s"), m._bo ? _TX("true") : _TX("false"));
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Boolean::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Boolean::dump");
    b_printf(_TX(" %s"), m._bo ? _TX("true") : _TX("false"));
}

//
// *** End of File ***
