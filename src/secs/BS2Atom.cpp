// $Id: BS2Atom.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Atom.cpp
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

#include "BS2DeclAtoms.h"
#include "BS2ItemHeader.h"
#include "BS2id.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Atom::BS2Atom(const BS2Atom& rhs) : BS2value(rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Atom::BS2Atom");
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
const BS2Atom& BS2Atom::operator=(const BS2Atom& rhs)
{
    if (this == &rhs)
        return *this;
    this->copy(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
const BS2Atom& BS2Atom::operator=(const BS2value& rhs)
{
    if (this == &rhs)
        return *this;
    this->copy(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void BS2Atom::copy(const BS2value& rhs)
{
    ((BS2value&)(*this)) = rhs;
}

//-----------------------------------------------------------------------------
// Dummy
//-----------------------------------------------------------------------------
void BS2Atom::get(BS2OStream&) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::get");
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
void BS2Atom::getStreamData(BYTE *) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::getStreamData");
}

//-----------------------------------------------------------------------------
// Dummy
//-----------------------------------------------------------------------------
void BS2Atom::set(BS2IStream&)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::set");
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
void BS2Atom::setStreamData(BYTE *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::setStreamData");
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
BS2Atom * BS2Atom::factory(BYTE *, size_t) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::factory");
    BS2Atom * atom = NULL;

    throw runtime_error(_TX("BS2Atom::factory - not supported"));

    return atom;
}

//-----------------------------------------------------------------------------
// Factory object by id (static)
//-----------------------------------------------------------------------------
BS2Atom * BS2Atom::factory(const b_id& id)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::id");
    BS2Atom * atom = static_cast<const BS2id&>(id).getAtom();
#if 0
    BS2Atom * atom;
    if (id.m_t == ATOM_ASCII) {
        atom = new BS2Ascii(*id.m._str);
    } else if (id.m_t == ATOM_UINT4) {
        atom = new BS2UInt4(id.m._ui);
    } else {
        TRACE_ERROR((_TX("Illegal variable type %d \n"), id.m_t));
        return NULL;
    }
#endif
    return atom;
}

//-----------------------------------------------------------------------------
// Factory object by value (static)
//-----------------------------------------------------------------------------
BS2Atom * BS2Atom::factory(const b_value& val)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::factory");
    BS2Atom * atom = static_cast<const BS2value&>(val).getAtom();
    return atom;
}

//-----------------------------------------------------------------------------
// Factory object by atom (static)
//-----------------------------------------------------------------------------
BS2Atom * BS2Atom::factory(const BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::factory");
    BS2Atom * clone = atom->getAtom();
    return clone;
}

//-----------------------------------------------------------------------------
// Get value
//-----------------------------------------------------------------------------
int BS2Atom::get(b_value& val)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::get");

    if (this == NULL)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("BS2Atom::get(value): ATOM is null\n")));
        return -1;
    }
    val = *this;
    return m_t;
}

//-----------------------------------------------------------------------------
int BS2Atom::get(b_id& id)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::get");

    if (this == NULL)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("BS2Atom::get(id): ATOM is null\n")));
        return -1;
    }
    if (m_t == ATOM_ASCII)
    {
        id = *(m._str);
    }
    else /* if (this->isInt() || this->isUInt()) */
    {
        id = this->getUInt();
    }
    return m_t;
}


//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Atom::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::dump");
}

//-----------------------------------------------------------------------------
// Atom Table by code
//-----------------------------------------------------------------------------
static BS2List    _list_atom;
static BS2Ascii   _ascii_atom;
static BS2Jis     _jis_atom;
static BS2Binary  _bin_atom;
static BS2Boolean _bool_atom;
static BS2Int1    _int1_atom;
static BS2Int2    _int2_atom;
static BS2Int4    _int4_atom;
static BS2Int8    _int8_atom;
static BS2UInt1   _uint1_atom;
static BS2UInt2   _uint2_atom;
static BS2UInt4   _uint4_atom;
static BS2UInt8   _uint8_atom;
static BS2Float4  _float4_atom;
static BS2Float8  _float8_atom;

BS2Atom * _atomTable[64] =
{
    &_list_atom,        //  0: ATOM_LIST
    NULL,               //  1:
    NULL,               //  2:
    NULL,               //  3:
    NULL,               //  4:
    NULL,               //  5:
    NULL,               //  6:
    NULL,               //  7:
    &_bin_atom,         //  8: ATOM_BINARY
    &_bool_atom,        //  9: ATOM_BOOLEAN
    NULL,               // 10:
    NULL,               // 11:
    NULL,               // 12:
    NULL,               // 13:
    NULL,               // 14:
    NULL,               // 15:
    &_ascii_atom,       // 16: ATOM_ASCII
    &_jis_atom,         // 17: ATOM_JIS
    NULL,               // 18:
    NULL,               // 19:
    NULL,               // 20:
    NULL,               // 21:
    NULL,               // 22:
    NULL,               // 23:
    &_int8_atom,        // 24: ATOM_INT8
    &_int1_atom,        // 25: ATOM_INT1
    &_int2_atom,        // 26: ATOM_INT2
    NULL,               // 27:
    &_int4_atom,        // 28: ATOM_INT4
    NULL,               // 29:
    NULL,               // 30:
    NULL,               // 31:
    &_float8_atom,      // 32: ATOM_FLOAT8
    NULL,               // 33:
    NULL,               // 34:
    NULL,               // 35:
    &_float4_atom,      // 36: ATOM_FLOAT4
    NULL,               // 37:
    NULL,               // 38:
    NULL,               // 39:
    &_uint8_atom,       // 40: ATOM_UINT8
    &_uint1_atom,       // 41: ATOM_UINT1
    &_uint2_atom,       // 42: ATOM_UINT2
    NULL,               // 43:
    &_uint4_atom,       // 44: ATOM_UINT4
    NULL,               // 45:
    NULL,               // 46:
    NULL,               // 47:
    NULL,               // 48:
    NULL,               // 49:
    NULL,               // 50:
    NULL,               // 51:
    NULL,               // 52:
    NULL,               // 53:
    NULL,               // 54:
    NULL,               // 55:
    NULL,               // 56:
    NULL,               // 57:
    NULL,               // 58:
    NULL,               // 59:
    NULL,               // 60:
    NULL,               // 61:
    NULL,               // 62:
    NULL,               // 63:
};

//-----------------------------------------------------------------------------
// Factory object by item header (static)
//-----------------------------------------------------------------------------
BS2Atom * BS2Atom::factory(const BS2ItemHeader& itemHeader)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Atom::factory");

    BS2Atom* atom = _atomTable[itemHeader.format() & 0x3F];
    
	/*if (atom == NULL)
        return atom;
    
	return atom->factory(itemHeader.data(), itemHeader.dataLength());*/

	if( atom )
		atom = atom->factory(itemHeader.data(), itemHeader.dataLength());

	return atom;
}

//
// *** End of File ***
