// $Id: BS2Binary.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Binary.cpp
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
#include "BS2Binary.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Binary::BS2Binary(BS2Binary const& rhs): BS2Atom(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Binary::BS2Binary");
    BS2Assert(rhs.m_t == ATOM_BINARY);
    this->copy((BS2Atom&)rhs);
}

//-----------------------------------------------------------------------------
BS2Binary::~BS2Binary()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Binary::BS2Binary");

}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Binary& BS2Binary::operator=(const BS2Binary& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::operator=");
    BS2Assert(m_t == ATOM_BINARY && rhs.m_t == ATOM_BINARY);
    if (this == &rhs)
        return *this;
    this->copy((BS2Atom&)rhs);
    return *this;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
void BS2Binary::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::set");
    BS2Assert(m_t == ATOM_BINARY);
    BS2ItemHeader itemHeader;
    buf >> itemHeader;
    int len = itemHeader.dataLength();
    if (m._bin != NULL)
    {
        free(m._bin);
        m._bin = NULL;
        m_q = 0;
    }

    if (len > 0)
    {
        if ((m._bin = (BYTE *)malloc(len)) != NULL)
        {
            buf.read((BCHAR *)m._bin, len);
            m_q = len;
        }
    }
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Binary::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::setStreamData");
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
void BS2Binary::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::get");
    BS2Assert(m_t == ATOM_BINARY);
    size_t size = m_q;
    int lenq = 0;
    if (size < 256)
        lenq = 1;
    else if (size < 65536)
        lenq = 2;
    else if (size < 16777216)
        lenq = 3;
    else
    {
        throw length_error(_TX("BS2Binary::get()"));
    }
    BS2ItemHeader itemHeader(ATOM_BINARY, lenq, size);
    buf << itemHeader;
    buf.write((char *)m._bin, size);
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Binary::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::getStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Binary::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::factory");
    BS2Binary * clone = new BS2Binary(data, len);
    return (BS2Atom *)clone;
}

//-----------------------------------------------------------------------------
BS2Atom * BS2Binary::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::replicate");

    BS2Binary * replica = new BS2Binary;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// Add
//-----------------------------------------------------------------------------
void BS2Binary::add(BS2Binary * binAtom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::add");

    int newSize = m_q + binAtom->m_q;
    BYTE * newBins = (BYTE *)realloc(m._bin, newSize);
    if (newBins == NULL)
    {
        throw runtime_error(_TX("BS2Binary::add(BS2Binary *) - lack memory"));
    }
    memcpy(newBins + m_q, binAtom->m._bin, binAtom->m_q);
    m_q = newSize;
    if (m._bin)
        free(m._bin);
    m._bin = newBins;
}

//-----------------------------------------------------------------------------
void BS2Binary::add(BYTE datum)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::add");

    int newSize = m_q + 1;
    BYTE * newBins = (BYTE *)realloc(m._bin, newSize);
    if (newBins == NULL)
    {
        throw runtime_error(_TX("BS2Binary::add(BYTE) - lack memory"));
    }
    *(newBins + m_q) = datum;
    m_q = newSize;
    if (m._bin)
        free(m._bin);
    m._bin = newBins;
}

//-----------------------------------------------------------------------------
// IO Stream
//-----------------------------------------------------------------------------
BS2IStream& operator>>(BS2IStream& is, BS2Binary& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2IStream::operator>>(BS2Binary&)");
    atom.set(is);
    return is;
}

BS2OStream& operator<<(BS2OStream& os, BS2Binary& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2OStream::operator<<(BS2Binary&)");
    atom.get(os);
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Binary::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::print");
    size_t size = m_q;
    BYTE * ptr  = m._bin;
    if (interp->print_xml())
    {
        if (size == 1)
            interp->printf(_TX(" 0x%02X "), *ptr);
        else
        {
            for (size_t i = 0; i < size; i++)
                interp->printf(_TX(" 0x%02X"), *(ptr + i));

            interp->printf(_TX(" "));
        }
    }
    else
    {
        if (size == 1)
            interp->printf(_TX(" 0x%02X"), *ptr);
        else
        {
            interp->printf(_TX(" {"));
            for (size_t i = 0; i < size; i++)
            {
                interp->printf(_TX(" 0x%02X"), *(ptr + i));
            }
            interp->printf(_TX(" }"));
        }
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Binary::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Binary::dump");
    if (m_q == 1)
        b_printf(_TX(" 0x%02X"), *m._bin);
    else
    {
        b_printf(_TX(" {"));
        for (int i = 0; i < m_q; i++)
        {
            b_printf(_TX(" 0x%02X"), *(m._bin + i));
        }
        b_printf(_TX(" }"));
    }
}

//
// *** End of File ***
