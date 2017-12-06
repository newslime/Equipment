// $Id: BS2Ascii.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Ascii.cpp
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
#include "BS2Ascii.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"

#include <iostream>

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Ascii::BS2Ascii(const BCHAR * str)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Ascii::BS2Ascii");
    initv(str);
}

//-----------------------------------------------------------------------------
BS2Ascii::BS2Ascii(const string& str)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Ascii::BS2Ascii");
    initv(str);
}

//-----------------------------------------------------------------------------
BS2Ascii::BS2Ascii(const BYTE * data, size_t len)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Ascii::BS2Ascii");
    if (data != NULL)
    {
        BCHAR * buf = new BCHAR[len + 1];
        memmove(buf, data, len);
        *(buf + len) = '\0';
        initv(buf);
        delete[] buf;
    }
    else
    {
        initNull();
    }
}

//-----------------------------------------------------------------------------
BS2Ascii::BS2Ascii(const BS2Ascii& rhs) : BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Ascii::BS2Ascii");
    BS2Assert(rhs.m_t == ATOM_ASCII);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Ascii& BS2Ascii::operator=(const BS2Ascii& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::operator=");
    BS2Assert(m_t == ATOM_ASCII && rhs.m_t == ATOM_ASCII);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Ascii& BS2Ascii::operator=(const string& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::operator=");
    BS2Assert(m_t == ATOM_ASCII);
    initv(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Ascii::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::set");
    BS2Assert(m_t == ATOM_ASCII);
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
            TRACE_ERROR((_TX("%T %N(%l): memory system lack.\n")));
    }
    else
        initNull();
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Ascii::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::setStreamData");
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Ascii::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::get");
    BS2Assert(m_t == ATOM_ASCII);
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
        throw length_error(_TX("BS2Ascii::get()"));
    }
    itemHeader.set(ATOM_ASCII, lenq, length);
    buf << itemHeader;
    if (length > 0)
        buf.write(vstr->c_str(), length);
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Ascii::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::getStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Ascii::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::factory");

    BS2Ascii * clone = new BS2Ascii(data, len);
    return (BS2Atom *)clone;
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Ascii::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::replicate");

    BS2Ascii * replica = new BS2Ascii;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Ascii& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Ascii& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Ascii::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::print");

    interp->printf(interp->print_xml() ? _TX(" %s ") : _TX(" \"%s\""),
                   m._str->c_str());
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Ascii::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Ascii::dump");

    b_printf(_TX(" \"%s\""), m._str->c_str());
}

//
// *** End of File ***

