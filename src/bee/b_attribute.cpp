// $Id: b_attribute.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_attribute.cpp
 *
 *  @author  Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2001-2004 BEE Co.,Ltd. All rights reserved.
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

#include "b_attribute.h"
#include "b_objtype.h"
#include "b_object.h"
#include "b_specification.h"
#include "b_attrdata.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_attribute::b_attribute(const BCHAR * name, int type, size_t size, int mode,
                         bool nodel)
        : m_name(name), m_userName(name), m_type(type), m_size(size),
          m_position(0), m_mode(mode), m_nodel(nodel), m_enumerator(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_attribute::b_attribute");

    switch (type)
    {
    case ATOM_ASCII:   m_init = "";                 break;
    case ATOM_BINARY:  m_init.set((BYTE *)NULL, 0); break;
    case ATOM_BOOLEAN: m_init = false;              break;
    case ATOM_INT1:    m_init = (char)0;            break;
    case ATOM_INT2:    m_init = (short)0;           break;
    case ATOM_INT4:    m_init = (int)0;             break;
    case ATOM_INT8:    m_init = (LONGLONG)0;        break;
    case ATOM_UINT1:   m_init = (BYTE)0;            break;
    case ATOM_UINT2:   m_init = (USHORT)0;          break;
    case ATOM_UINT4:   m_init = (UINT)0;            break;
    case ATOM_UINT8:   m_init = (ULONGLONG)0;       break;
    case ATOM_FLOAT4:  m_init = (float)0.0;         break;
    case ATOM_FLOAT8:  m_init = (double)0.0;        break;
    case ATOM_STRUCT:
        {
            b_valvect vv;
            m_init = vv;
            m_init.m_t = ATOM_STRUCT;
        }
        break;
    case ATOM_VECTOR:
        {
            b_valvect vv;
            m_init = vv;
            m_init.m_t = ATOM_VECTOR;
        }
        break;
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
b_attribute::b_attribute(b_objtype& cls, const BCHAR * name, int type,
                         size_t size, int mode, bool nodel)
        : m_name(name), m_userName(name), m_type(type), m_size(size),
          m_position(0), m_mode(mode), m_nodel(nodel), m_enumerator(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_attribute::b_attribute");

    switch (type)
    {
    case ATOM_ASCII:   m_init = "";                 break;
    case ATOM_BINARY:  m_init.set((BYTE *)NULL, 0); break;
    case ATOM_BOOLEAN: m_init = false;              break;
    case ATOM_INT1:    m_init = (char)0;            break;
    case ATOM_INT2:    m_init = (short)0;           break;
    case ATOM_INT4:    m_init = (int)0;             break;
    case ATOM_INT8:    m_init = (LONGLONG)0;        break;
    case ATOM_UINT1:   m_init = (BYTE)0;            break;
    case ATOM_UINT2:   m_init = (USHORT)0;          break;
    case ATOM_UINT4:   m_init = (UINT)0;            break;
    case ATOM_UINT8:   m_init = (ULONGLONG)0;       break;
    case ATOM_FLOAT4:  m_init = (float)0.0;         break;
    case ATOM_FLOAT8:  m_init = (double)0.0;        break;
    case ATOM_STRUCT:
        {
            b_valvect vv;
            m_init = vv;
            m_init.m_t = ATOM_STRUCT;
        }
        break;
    case ATOM_VECTOR:
        {
            b_valvect vv;
            m_init = vv;
            m_init.m_t = ATOM_VECTOR;
        }
        break;
    default:
        break;
    }

    cls.add(this);
}

//-----------------------------------------------------------------------------
b_attribute::~b_attribute()
{
    TRACE_FUNCTION(TRL_LOW, "b_attribute::b_attribute");

    for (size_t i = 0; i < m_attrs.size(); i++)
    {
        b_attribute * mbr = m_attrs[i];
        if (! mbr->nodel())  // can't delete static attribute
        {
           //@ delete mbr;
        }
    }
}

//-----------------------------------------------------------------------------
// Create instance data of attribute
//-----------------------------------------------------------------------------
b_attrdata * b_attribute::instance(const b_object * obj) const
{
    TRACE_FUNCTION(TRL_LOW, "b_attribute::instance");

    b_attrdata * val = new b_attrdata(obj, this);

    return val;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void b_attribute::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  name = %s(%s), "), m_name.c_str(),
                                                        m_userName.c_str()));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("type = 0x%x, "), m_type));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("size = %d, "), m_size));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("position = %d, "), m_position));
    string value;
    m_init.get(value);
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("initval = %s\n"), value.c_str()));
}

//
// <<< Vector >>>
//
//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_vector::b_vector(const BCHAR * name, b_attribute * attr, size_t q, int mode,
                   bool nodel)
        : b_attribute(name, ATOM_VECTOR, q, mode, nodel)
{
    if (attr != NULL)
    {
        m_attrs.push_back(attr);
        b_valvect vv;
        vv.push_back(const_cast<b_value *>(&(attr->getInitVal())));
        m_init = vv;
    }
}

//-----------------------------------------------------------------------------
b_vector::b_vector(b_objtype& cls, const BCHAR * name, b_attribute * attr,
                   size_t q, int mode, bool nodel)
        : b_attribute(name, ATOM_VECTOR, q, mode, nodel)
{
    if (attr != NULL)
    {
        m_attrs.push_back(attr);
        b_valvect vv;
        vv.push_back(const_cast<b_value *>(&(attr->getInitVal())));
        m_init = vv;
    }
    cls.add(this);
}

//-----------------------------------------------------------------------------
b_vector::b_vector(b_struct& stt, const BCHAR * name, b_attribute * attr,
                   size_t q, int mode, bool nodel)
        : b_attribute(name, ATOM_VECTOR, q, mode, nodel)
{
    if (attr != NULL)
    {
        m_attrs.push_back(attr);
        b_valvect vv;
        vv.push_back(const_cast<b_value *>(&(attr->getInitVal())));
        m_init = vv;
    }
    stt.add(this);
}

//-----------------------------------------------------------------------------
// Create instance data of attribute
//-----------------------------------------------------------------------------
b_attrdata * b_vector::instance(const b_object * obj) const
{
    TRACE_FUNCTION(TRL_LOW, "b_vector::instance");

    b_attrdata * val = new b_attrdata(obj, this);

    return val;
}

//
// <<< Struct >>>
//
//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_struct::b_struct(const BCHAR * name, int mode, bool nodel)
        : b_attribute(name, ATOM_STRUCT, 0, mode, nodel)
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::b_struct");

    return ;
}

//-----------------------------------------------------------------------------
b_struct::b_struct(b_objtype& cls, const BCHAR * name, int mode, bool nodel)
        : b_attribute(name, ATOM_STRUCT, 0, mode, nodel)
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::b_struct");

    cls.add(this);
}

//-----------------------------------------------------------------------------
b_struct::b_struct(b_struct& stt, const BCHAR * name, int mode, bool nodel)
        : b_attribute(name, ATOM_STRUCT, 0, mode, nodel)
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::b_struct");

    stt.add(this);
}

//-----------------------------------------------------------------------------
// Create instance data of attribute
//-----------------------------------------------------------------------------
b_attrdata * b_struct::instance(const b_object * obj) const
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::instance");

    b_attrdata * val = new b_attrdata(obj, this);

    return val;
}

//-----------------------------------------------------------------------------
// Add member attributes
//-----------------------------------------------------------------------------
extern bool comp_position(b_attribute * lhs, b_attribute * rhs);

//
void b_struct::add(b_attribute * attr)
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::add");

    BEEAssert(attr != NULL);
    m_attrs.push_back(attr);
    if (attr->position() == 0)
    {
        attr->position(m_attrs.size() - 1);     // Set position of struct
    }
    b_value * v = new b_value(attr->getInitVal());
    m_init.m._vec->push_back(v);

    sort(m_attrs.begin(), m_attrs.end(), comp_position);

    return ;
}

//-----------------------------------------------------------------------------
void b_struct::add(b_attribute * attrs[])
{
    TRACE_FUNCTION(TRL_LOW, "b_struct::add");

    BEEAssert(attrs != NULL);
    int i = 0;
    while (attrs[i] != NULL)
    {
        b_attribute * attr = attrs[i++];
        m_attrs.push_back(attr);
        if (attr->position() == 0)
        {
            attr->position(m_attrs.size() - 1);     // Set position of struct
        }
        b_value * v = new b_value(attr->getInitVal());
        m_init.m._vec->push_back(v);
    }

    sort(m_attrs.begin(), m_attrs.end(), comp_position);

    return ;
}

