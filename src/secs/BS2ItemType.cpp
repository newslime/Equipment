// $Id: BS2ItemType.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ItemType.cpp
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

#include "BS2ItemType.h"
#include "BS2Item.h"
#include "BS2ListItem.h"
#include "BS2Interpreter.h"

//-----------------------------------------------------------------------------
// Convert format bits to name.
//-----------------------------------------------------------------------------
static string _formatNames(UINT format)
{
    TRACE_FUNCTION(TRL_LOW, "_formatNames");

    BCHAR buf[256];
    buf[0] = '\0';
    if ((format & BS2ItemType::LIST_BIT) != 0)
    {
        _tcscat(buf, _TX("LIST "));
    }
    if ((format & BS2ItemType::BINARY_BIT) != 0)
    {
        _tcscat(buf, _TX("BIN "));
    }
    if ((format & BS2ItemType::BOOLEAN_BIT) != 0)
    {
        _tcscat(buf, _TX("BOOLEAN "));
    }
    if ((format & BS2ItemType::ASCII_BIT) != 0)
    {
        _tcscat(buf, _TX("ASCII "));
    }
    if ((format & BS2ItemType::JIS_BIT) != 0)
    {
        _tcscat(buf, _TX("JIS "));
    }
    if ((format & BS2ItemType::UNICODE_BIT) != 0)
    {
        _tcscat(buf, _TX("UNICODE "));
    }

    if (((format & BS2ItemType::INT1_BIT) != 0) &&
        ((format & BS2ItemType::INT2_BIT) != 0) &&
        ((format & BS2ItemType::INT4_BIT) != 0) &&
        ((format & BS2ItemType::INT8_BIT) != 0))
    {
        _tcscat(buf, _TX("I() "));
    }
    else
    {
        if ((format & BS2ItemType::INT1_BIT) != 0)
            _tcscat(buf, _TX("I1 "));
        if ((format & BS2ItemType::INT2_BIT) != 0)
            _tcscat(buf, _TX("I2 "));
        if ((format & BS2ItemType::INT4_BIT) != 0)
            _tcscat(buf, _TX("I4 "));
        if ((format & BS2ItemType::INT8_BIT) != 0)
            _tcscat(buf, _TX("I8 "));
    }

    if (((format & BS2ItemType::FLOAT4_BIT) != 0) &&
        ((format & BS2ItemType::FLOAT8_BIT) != 0))
    {
        _tcscat(buf, _TX("F() "));
    }
    else
    {
        if ((format & BS2ItemType::FLOAT4_BIT) != 0)
            _tcscat(buf, _TX("F4 "));
        if ((format & BS2ItemType::FLOAT8_BIT) != 0)
            _tcscat(buf, _TX("F8 "));
    }

    if (((format & BS2ItemType::UINT1_BIT) != 0) &&
        ((format & BS2ItemType::UINT2_BIT) != 0) &&
        ((format & BS2ItemType::UINT4_BIT) != 0) &&
        ((format & BS2ItemType::UINT8_BIT) != 0))
    {
        _tcscat(buf, _TX("U() "));
    }
    else
    {
        if ((format & BS2ItemType::UINT1_BIT) != 0)
            _tcscat(buf, _TX("U1 "));
        if ((format & BS2ItemType::UINT2_BIT) != 0)
            _tcscat(buf, _TX("U2 "));
        if ((format & BS2ItemType::UINT4_BIT) != 0)
            _tcscat(buf, _TX("U4 "));
        if ((format & BS2ItemType::UINT8_BIT) != 0)
            _tcscat(buf, _TX("U8 "));
    }
    buf[_tcslen(buf) - 1] = _TX('\0');
    return buf;
}

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ItemType::BS2ItemType(const string& name, UINT format, int defaultFormat)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemType::BS2ItemType");
    m_name = name;
    m_formats = format;
    m_defaultFormat = defaultFormat;
    m_formatNames = _formatNames(format);
    m_list = false;
    m_isObj = false;
}

//-----------------------------------------------------------------------------
// Convert format bit to number (static)
//-----------------------------------------------------------------------------
int BS2ItemType::bitToNum(UINT fmtbit)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemType::bitToNum");
    int result = -1;
    if ((fmtbit & BS2ItemType::LIST_BIT) != 0)
    {
        result = ATOM_LIST;
    }
    else if ((fmtbit & BS2ItemType::BINARY_BIT) != 0)
    {
        result = ATOM_BINARY;
    }
    else if ((fmtbit & BS2ItemType::BOOLEAN_BIT) != 0)
    {
        result = ATOM_BOOLEAN;
    }
    else if ((fmtbit & BS2ItemType::ASCII_BIT) != 0)
    {
        result = ATOM_ASCII;
    }
    else if ((fmtbit & BS2ItemType::JIS_BIT) != 0)
    {
        result = ATOM_JIS;
    }
    else if ((fmtbit & BS2ItemType::UNICODE_BIT) != 0)
    {
        result = ATOM_UNICODE;
    }
    else if ((fmtbit & BS2ItemType::INT1_BIT) != 0)
    {
        result = ATOM_INT1;
    }
    else if ((fmtbit & BS2ItemType::INT2_BIT) != 0)
    {
        result = ATOM_INT2;
    }
    else if ((fmtbit & BS2ItemType::INT4_BIT) != 0)
    {
        result = ATOM_INT4;
    }
    else if ((fmtbit & BS2ItemType::INT8_BIT) != 0)
    {
        result = ATOM_INT8;
    }
    else if ((fmtbit & BS2ItemType::FLOAT4_BIT) != 0)
    {
        result = ATOM_FLOAT4;
    }
    else if ((fmtbit & BS2ItemType::FLOAT8_BIT) != 0)
    {
        result = ATOM_FLOAT8;
    }
    else if ((fmtbit & BS2ItemType::UINT1_BIT) != 0)
    {
        result = ATOM_UINT1;
    }
    else if ((fmtbit & BS2ItemType::UINT2_BIT) != 0)
    {
        result = ATOM_UINT2;
    }
    else if ((fmtbit & BS2ItemType::UINT4_BIT) != 0)
    {
        result = ATOM_UINT4;
    }
    else if ((fmtbit & BS2ItemType::UINT8_BIT) != 0)
    {
        result = ATOM_UINT8;
    }
    return result;
}

//-----------------------------------------------------------------------------
// Is format avail the item ?
//-----------------------------------------------------------------------------
bool BS2ItemType::availFormat(int form) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemType::availFormat");
    bool result = false;

    if ((form == ATOM_ASCII && (m_formats & BS2ItemType::ASCII_BIT)) ||
        (form == ATOM_UINT2 && (m_formats & BS2ItemType::UINT2_BIT)) ||
        (form == ATOM_UINT4 && (m_formats & BS2ItemType::UINT4_BIT)) ||
        (form == ATOM_INT2  && (m_formats & BS2ItemType::INT2_BIT)) ||
        (form == ATOM_INT4  && (m_formats & BS2ItemType::INT4_BIT)) ||
        (form == ATOM_BINARY && (m_formats & BS2ItemType::BINARY_BIT)) ||
        (form == ATOM_BOOLEAN && (m_formats & BS2ItemType::BOOLEAN_BIT)) ||
        (form == ATOM_INT1  && (m_formats & BS2ItemType::INT1_BIT)) ||
        (form == ATOM_INT8  && (m_formats & BS2ItemType::INT8_BIT)) ||
        (form == ATOM_UINT1 && (m_formats & BS2ItemType::UINT1_BIT)) ||
        (form == ATOM_UINT8 && (m_formats & BS2ItemType::UINT8_BIT)) ||
        (form == ATOM_LIST  && (m_formats & BS2ItemType::LIST_BIT)) ||
        (form == ATOM_JIS   && (m_formats & BS2ItemType::JIS_BIT)) ||
        (form == ATOM_FLOAT4 && (m_formats & BS2ItemType::FLOAT4_BIT)) ||
        (form == ATOM_FLOAT8 && (m_formats & BS2ItemType::FLOAT8_BIT)))
    {
        result = true;
    }
    return result;
}

//-----------------------------------------------------------------------------
// Factory entity
//-----------------------------------------------------------------------------
BS2Item * BS2ItemType::factory(BS2Atom * atom)
{
    BS2Item * item = new BS2Item(this, atom);
    return item;
}

//-----------------------------------------------------------------------------
BS2Item * BS2Itemtypes::factory(BS2Atom * atom)
{
    if (atom->isList())
    {
        BS2ListItem * litem = new BS2ListItem(atom);
        return litem;
    }
    else
    {   // Probabry, object's value
        BS2Item * item = new BS2Item(this, atom);
        return item;
    }
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2ItemType::print(BS2InterpBase * interp)
{
    interp->printf(_TX("%s: %s (0x%x)\n"), m_name.c_str(),
                                           m_formatNames.c_str(),
                                           m_defaultFormat);
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2ItemType::dump() const
{
    b_printf(_TX("%s: %s (0x%x)\n"), m_name.c_str(), m_formatNames.c_str(),
                                     m_defaultFormat);
}


//
// *** End of File ***
