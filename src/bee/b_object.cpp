// $Id: b_object.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_object.cpp
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

#include "b_objtype.h"
#include "b_object.h"
#include "b_attrdata.h"
#include "b_specification.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_object::b_object(b_objtype * cls, const BCHAR * name)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::b_object");

    m_objtype = cls;
    if (name == NULL)
    {   // type + id
        cls->makeObjID(m_name);
    }
    else
    {   // type + name
        m_name = name;
    }
    m_spec = cls->typeName();
    m_spec += _TX(":");
    m_spec += m_name;

    b_attributes::iterator iter = cls->m_attrs.begin();
    for ( ; iter != cls->m_attrs.end(); iter++)
    {
        b_attribute * mbrattr = *iter;
        b_attrdata *  adata = mbrattr->instance(this);
        m_values.push_back(adata);
        if (mbrattr->name() == _TX("ObjType"))
        {
            b_value objtype(m_objtype->typeName());
            adata->set(objtype);
        }
        else if (mbrattr->name() == _TX("ObjID"))
        {
            b_value objid(m_name.c_str());
            adata->set(objid);
        }
    }

    return ;
}

//-----------------------------------------------------------------------------
b_object::b_object(const b_object& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::b_object");

    this->copy(rhs);
    return ;
}

//-----------------------------------------------------------------------------
b_object::~b_object()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::~b_object");
    AttrDataVect::iterator iter;
    for (iter = m_values.begin(); iter != m_values.end(); iter++)
    {
        b_attrdata * data = *iter;
        delete data;
    }
    m_objtype->remove(this);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
b_object& b_object::operator=(const b_object& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::operator=");
    if (this == &rhs)
        return *this;
    this->copy(rhs);
    return *this;
}

//-----------------------------------------------------------------------------
void b_object::copy(const b_object& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::copy");

    m_objtype = rhs.m_objtype;
    m_objtype->makeObjID(m_name);
    m_spec = m_objtype->typeName();
    m_spec += ":";
    m_spec += m_name;

    m_values.clear();
    AttrDataVect::const_iterator iter;
    for (iter = rhs.m_values.begin(); iter != rhs.m_values.end(); iter++)
    {
        b_attrdata * data = *iter;
        b_attrdata * dest = new b_attrdata(*data);
        m_values.push_back(dest);
    }
}

//-----------------------------------------------------------------------------
// Set value at attribute
//-----------------------------------------------------------------------------
const b_attribute * b_object::findAttr(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_object::findAttr");
    const b_attribute * result = m_objtype->find(name.c_str());
    return result;
}

//-----------------------------------------------------------------------------
b_attrdata * b_object::findData(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_object::findData");

    const b_attribute * attr = m_objtype->find(name.c_str());
    if (attr == NULL)
    {
        return NULL;
    }

    return m_values[attr->position()];
}

//-----------------------------------------------------------------------------
b_attrdata * b_object::findDataByUser(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_object::findData");

    const b_attribute * attr = m_objtype->findByUser(name);
    if (attr == NULL)
    {
        return NULL;
    }

    return m_values[attr->position()];
}

//-----------------------------------------------------------------------------
// Set attribute data
//-----------------------------------------------------------------------------
int b_object::set(const b_object& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::set");
    BEEAssert(m_objtype == rhs.m_objtype);

    for (size_t i = 0; i < rhs.m_values.size(); i++)
    {
        b_attrdata * srcdata = rhs.m_values[i];
        b_attrdata * destdata = m_values[i];
        destdata->set(srcdata->get());
            //
            // Don't check atribute name
            //
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Compare attribute data
//-----------------------------------------------------------------------------
bool b_object::isEqual(const b_object& rhs)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "b_object::set");
    BEEAssert(m_objtype == rhs.m_objtype);

    bool result = true;
    for (size_t i = 0; i < rhs.m_values.size(); i++)
    {
        b_attrdata * srcdata = rhs.m_values[i];
        b_attrdata * destdata = m_values[i];
        if (! (*srcdata == *destdata))
        {
            result = false;
        }
    }
    return result;
}



//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void b_object::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("<object name=\"%s\" class=\"%s\">\n"),
                         m_name.c_str(), m_objtype->typeName()));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  <attrdata>\n")));
    AttrDataVect::const_iterator iter;
    for (iter = m_values.begin(); iter != m_values.end(); iter++)
    {
        const b_attrdata * data = *iter;
        data->dump();
    }
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  </attrdata>\n</object>\n")));
}


