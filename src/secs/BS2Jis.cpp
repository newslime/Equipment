// $Id: BS2Jis.cpp,v 1.7 2004/06/27 08:01:56 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Jis.cpp
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
#include "BS2Jis.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"

#include <iostream>

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Jis::BS2Jis(BS2Jis const& rhs): BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Jis::BS2Jis");
    BS2Assert(rhs.m_t == ATOM_JIS);
    this->copy((BS2Atom&)rhs);
}

//-----------------------------------------------------------------------------
BS2Jis::BS2Jis(BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Jis::BS2Jis");
    if (data != NULL)
    {
        initv((len > 0) ? (BCHAR *)data : "", len);
    }
    else
    {
        initNull();
    }
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Jis& BS2Jis::operator=(const BS2Jis& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::operator=");
    BS2Assert(m_t == ATOM_JIS && rhs.m_t == ATOM_JIS);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Jis::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::set");
    BS2Assert(m_t == ATOM_JIS);

    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    int len = itemHeader.dataLength();
    if (len > 0)
    {
        BCHAR * data = new BCHAR[len + 1];
        if (data != NULL)
        {
            buf.read(data, len);
            data[len] = '\0';
            initv(data);
            delete [] data;
        }
        else
            ACE_ERROR((LM_ERROR, "%T %N(%l): memory system lack.\n"));
    }
    else
        initNull();
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Jis::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::setStreamData");
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Jis::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::get");
    BS2Assert(m_t == ATOM_JIS);

    BS2ItemHeader itemHeader;
    string * vstr = m._str;
    int length = (this->haveData() && vstr != NULL) ? vstr->length() : 0;
    int lenq = 0;
    if (length < 256)
        lenq = 1;
    else if (length < 65536)
        lenq = 2;
    else if (length < 16777216)
        lenq = 3;
    else
    {
        throw length_error(_TX("BS2Jis::get()"));
    }
    itemHeader.set(ATOM_JIS, lenq, length);
    buf << itemHeader;
    if (length > 0)
        buf.write(vstr->c_str(), length);
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Jis::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::getStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Jis::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::factory");
    BS2Jis * clone = new BS2Jis(data, len);
    return (BS2Atom *)clone;
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Jis::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::replicate");

    BS2Jis * replica = new BS2Jis;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Jis& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>");
    atom.set(is);
    return is;
}

//-----------------------------------------------------------------------------
BS2OStream& operator<<(BS2OStream& os, BS2Jis& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Jis::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::print");
    if (interp->print_xml())
    {
        interp->printf(_TX(" %s "), m._str->c_str());
    }
    else
    {
        interp->printf(_TX(" \"%s\""), m._str->c_str());
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Jis::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Jis::dump");

    b_printf(_TX(" \"%s\""), m._str->c_str());
}

//
// *** End of File ***
