// $Id: BS2value.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2value.cpp
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 1999-2004 BEE Co.,Ltd. All rights reserved.
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

#include "BS2value.h"
#include "BS2TimeValue.h"
#include "BS2DeclAtoms.h"

#ifdef _DEBUG
#define WARNING_TYPE_CONVERT(x) \
   ACE_ERROR((LM_ERROR, ACE_TEXT("Can't convert variable type to %s.\n"), (x)))
#else
#define WARNING_TYPE_CONVERT(x)
#endif

extern int ascii2tm(const BCHAR * timestr, struct tm * outm);

BS2value BS2value::zero;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
void BS2value::copy(const BS2Atom& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2value::copy");
    *this = const_cast<BS2Atom&>(rhs).getValue();
}

//-----------------------------------------------------------------------------
// Get value for SECS Message
//-----------------------------------------------------------------------------
BS2Atom * BS2value::getAtom() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2value::getAtom");
    BS2Atom * atom;
    switch (m_t)
    {
    case ATOM_ASCII:
        atom = (m._str != NULL) ? new BS2Ascii(*m._str) : new BS2Ascii();
        break;
    case ATOM_BOOLEAN:
        atom = new BS2Boolean(m._bo);
        break;
    case ATOM_UINT4:
        atom = new BS2UInt4(m._ui);
        break;
    case ATOM_INT4:
        atom = new BS2Int4(m._in);
        break;
    case ATOM_UINT2:
        atom = new BS2UInt2(m._us);
        break;
    case ATOM_INT2:
        atom = new BS2Int2(m._sh);
        break;
    case ATOM_UINT1:
        atom = new BS2UInt1(m._uc);
        break;
    case ATOM_INT1:
        atom = new BS2Int1(m._ch);
        break;
    case ATOM_FLOAT4:
        atom = new BS2Float4(m._fl);
        break;
    case ATOM_FLOAT8:
        atom = new BS2Float8(m._do);
        break;
    case ATOM_BINARY:
        atom = new BS2Binary(m._bin, m_q);
        break;
    case ATOM_LIST:
        {
            BS2List * listatom = new BS2List();
            if (m._vec != NULL)
            {
                for (size_t i = 0; i < m._vec->size(); i++)
                {
                    BS2value vp(*((*m._vec)[i]));
                    BS2Atom * mbrAtom = vp.getAtom();
                    listatom->add(mbrAtom);
                }
            }
            atom = (BS2Atom *)listatom;
        }
        break;
    default:
        TRACE_ERROR((_TX("Illegal variable type %d \n"), m_t));
        atom = NULL;
    }
    return atom;
}

//-----------------------------------------------------------------------------
BS2Atom * BS2value::getAtom(int type) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2value::getAtom");
    BS2Atom * atom;

    if (type == ATOM_ASCII) {
        if (m_t == ATOM_ASCII) {
            if (m._str != NULL)
                atom = new BS2Ascii(*m._str);
            else
                atom = new BS2Ascii();
        } else {
            string valbuf = toString();
            atom = new BS2Ascii(valbuf.c_str());
        }
    } else if (type == ATOM_INT4) {
        if (m_t == ATOM_INT4) {
            atom = new BS2Int4(m._in);
        } else {
            WARNING_TYPE_CONVERT("int4");
            int ival = getInt();
            atom = new BS2Int4(ival);
        }
    } else if (type == ATOM_UINT4) {
        if (m_t == ATOM_UINT4) {
            atom = new BS2UInt4(m._ui);
        } else {
            WARNING_TYPE_CONVERT("uint4");
            u_int ival = getInt();
            atom = new BS2UInt4(ival);
        }
    } else if (type == ATOM_INT1) {
        if (m_t == ATOM_INT1) {
            atom = new BS2Int1(m._ch);
        } else {
            WARNING_TYPE_CONVERT("int1");
            int ival = getInt();
            atom = new BS2Int1((BCHAR)ival);
        }
    } else if (type == ATOM_INT2) {
        if (m_t == ATOM_INT2) {
            atom = new BS2Int2(m._sh);
        } else {
            WARNING_TYPE_CONVERT("int2");
            int ival = getInt();
            atom = new BS2Int2((short)ival);
        }
    } else if (type == ATOM_INT8) {
        if (m_t == ATOM_INT8) {
            atom = new BS2Int8(m._llo);
        } else if (m_t == ATOM_UINT8) {
            atom = new BS2Int8((LONGLONG)m._ull);
        } else {
            WARNING_TYPE_CONVERT("int8");
            u_int ival = getUInt();
            atom = new BS2Int8((LONGLONG)ival);
        }
    } else if (type == ATOM_UINT1) {
        if (m_t == ATOM_UINT1) {
            atom = new BS2UInt1((BYTE)m._uc);
        } else {
            WARNING_TYPE_CONVERT("uint1");
            u_int ival = getUInt();
            atom = new BS2UInt1((BYTE)ival);
        }
    } else if (type == ATOM_UINT2) {
        if (m_t == ATOM_UINT2) {
            atom = new BS2UInt2(m._us);
        } else {
            WARNING_TYPE_CONVERT("uint2");
            u_int ival = getUInt();
            atom = new BS2UInt2((u_short)ival);
        }
    } else if (type == ATOM_UINT8) {
        if (m_t == ATOM_UINT8) {
            atom = new BS2UInt8(m._ull);
        } else if (m_t == ATOM_INT8) {
            atom = new BS2UInt8((ULONGLONG)m._llo);
        } else {
            WARNING_TYPE_CONVERT("uint8");
            u_int ival = getUInt();
            atom = new BS2UInt8((ULONGLONG)ival);
        }
    } else if (type == ATOM_BOOLEAN) {
        if (m_t == ATOM_BOOLEAN) {
            atom = new BS2Boolean(m._bo);
        } else {
            WARNING_TYPE_CONVERT("boolean");
            bool ival = getBool();
            atom = new BS2Boolean(ival);
        }
    } else if (type == ATOM_BINARY) {
        if (m_t == ATOM_BINARY) {
            atom = new BS2Binary(m._bin, m_q);
        } else if (m_t == ATOM_ASCII) {
            atom = new BS2Binary((BYTE *)m._str->c_str(), m._str->size());
        } else {
            TRACE_ERROR((_TX("Do not support type %d \n"), type));
            return NULL;
        }
    } else if (type == ATOM_FLOAT4) {
        if (m_t == ATOM_FLOAT4) {
            atom = new BS2Float4(m._fl);
        } else {
            TRACE_ERROR((_TX("Can't convert variable type to float.\n")));
            return NULL;
        }
    } else if (type == ATOM_FLOAT8) {
        if (m_t == ATOM_FLOAT8) {
            atom = new BS2Float8(m._do);
        } else {
            TRACE_ERROR((_TX("Can't convert variable type to double.\n")));
            return NULL;
        }
    } else if (type == ATOM_LIST) {
        if (m_t == ATOM_LIST && m._vec != NULL) {
            BS2List * listatom = new BS2List();
            for (size_t i = 0; i < m._vec->size(); i++)
            {
                BS2value vp(*((*m._vec)[i]));
                BS2Atom * mbrAtom = vp.getAtom();
                listatom->add(mbrAtom);
            }
            atom = (BS2Atom *)listatom;
        } else if (m_t == ATOM_LIST) {
            BS2List * listatom = new BS2List();
            atom = (BS2Atom *)listatom;
        } else {
            TRACE_ERROR((_TX("Can't convert variable type to list.\n")));
            return NULL;
        }
    } else {
        TRACE_ERROR((_TX("Illegal variable type %d \n"), type));
        return NULL;
    }

    return atom;
}

//-----------------------------------------------------------------------------
// Set from time value.
//-----------------------------------------------------------------------------
BS2value& BS2value::operator=(BS2TimeValue& tv)
{
    TRACE_FUNCTION(TRL_LOW, "BS2value::operator=");

    clean();
    m_t = ATOM_UINT4; m_q = sizeof(u_int);
    m._ui = (UINT)tv.sec();
    return *this;
}

//-----------------------------------------------------------------------------
BS2TimeValue BS2value::getTimeValue() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2value::operator ACE_Time_Value");
    ACE_Time_Value tv(0);
    if (m_t == ATOM_ASCII) 
	{
        struct tm tmval;
        ascii2tm(m._str->c_str(), &tmval);
        tv.set((double)mktime(&tmval));
    } 
	else if (m_t == ATOM_UINT4) 
	{
        tv.sec(m._ui);
    } 
	else if (m_t == ATOM_INT4) 
	{
        tv.sec(m._in);
    } 
	else if (m_t == ATOM_FLOAT4) 
	{
        tv.set((double)m._fl);
    } 
	else if (m_t == ATOM_FLOAT8) 
	{
        tv.set(m._do);
    } 
	else 
	{
        ACE_ERROR((LM_ERROR,
                  ACE_TEXT("When cast, illegal variable type %d \n"), m_t));
    }
    return tv;
}

