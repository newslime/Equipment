// $Id: b_objtype.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_objtype.cpp
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
#include "b_attribute.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_objtype::b_objtype(const string& name, b_specification * spec)
        : m_name(name), m_userName(name), m_seed(1)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::b_objtype");
    if (spec == NULL)
    {
        m_spec = ObjSpec::instance();
    }
    else
    {
        m_spec = spec;
    }
    m_spec->add(this);
}

//-----------------------------------------------------------------------------
b_objtype::~b_objtype()
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::~b_objtype");

    // Release all objects
    b_objectMap::iterator iter = m_objs.begin();
    for ( ; iter != m_objs.end(); iter++)
    {
        b_object * obj = (*iter).second;
        delete obj;
        m_spec->remove(obj);
    }
    m_objs.clear();

    // Release all attributes
    for (size_t i = 0; i < m_attrs.size(); i++)
    {
        b_attribute * attr = m_attrs[i];
        if (! attr->nodel())     // can't delete static attributes
        {
            //@ delete attr;
        }
    }
    m_attrs.clear();
    m_dict.clear();

    m_spec->remove(this);

}

//-----------------------------------------------------------------------------
// Make object idenntifier
//-----------------------------------------------------------------------------
int b_objtype::makeObjID(string& retid)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::makeObjID");

    BCHAR buf[256+2];
    u_int oid = m_seed++;
    _stprintf(buf, _TX("%s:%u"), m_name.c_str(), oid);
    retid = buf;
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Add attribute
//-----------------------------------------------------------------------------
bool comp_position(b_attribute * lhs, b_attribute * rhs)
{
    return (lhs->position() < rhs->position());
}

//-----------------------------------------------------------------------------
int b_objtype::add(b_attribute * attr)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::add");

    m_attrs.push_back(attr);
    if (attr->position() == 0)
    {
        attr->position(m_attrs.size() - 1);     // Set position of object
    }
    this->entryAttrName(attr);

    sort(m_attrs.begin(), m_attrs.end(), comp_position);

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
int b_objtype::entryAttrName(b_attribute * attr)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::entryAttrName");

    m_dict.insert(b_attrPair(attr->name(), attr));
    b_attribute * mbrAttr;
    if (attr->isStruct() || attr->isVector())
    {
        for (size_t i = 0; i < attr->m_attrs.size(); i++)
        {
            mbrAttr = attr->m_attrs[i];
            this->entryAttrName(mbrAttr);
        }
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Find attribute by name
//-----------------------------------------------------------------------------
const b_attribute * b_objtype::find(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::find");

    b_attrMap::const_iterator iter = m_dict.find(name);
    if (iter == m_dict.end())
    {
        return NULL;
    }
    const b_attribute * result = (*iter).second;
    return result;
}

//-----------------------------------------------------------------------------
const b_attribute * b_objtype::findByUser(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::findByUser");

    const b_attribute * attr;
    b_attributes::const_iterator at_iter = m_attrs.begin();
    for ( ; at_iter != m_attrs.end(); at_iter++)
    {
        attr = *at_iter;
        if (attr->m_userName == name)
        {   // found user's name attribute
            return attr;
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
// Create object
//-----------------------------------------------------------------------------
b_object * b_objtype::instance(const BCHAR * name)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::instance");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, NULL,
                     10, 200);

    b_object * obj = new b_object(this, name);
    m_objs.insert(b_objectPair(obj->objid(), obj));
    m_spec->addObject(obj);
    return obj;
}

//-----------------------------------------------------------------------------
int b_objtype::entry(b_object * obj)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::entry");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, BEE_ERROR,
                     10, 200);

    m_objs.insert(b_objectPair(obj->objid(), obj));
    m_spec->addObject(obj);
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
b_object * b_objtype::findObject(const string& name)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::findObject");

    string objid = _TX("");
    if (name.find(':') == string::npos)
    {
        objid += this->m_name;
        objid += _TX(":");
    }
    objid += name;

    b_objectMap::const_iterator iter = m_objs.find(objid);
    if (iter == m_objs.end())
    {
        return NULL;
    }
    b_object * result = (*iter).second;
    return result;
}


//-----------------------------------------------------------------------------
// Remove object
//-----------------------------------------------------------------------------
int b_objtype::remove(b_object * obj)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::remove");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, BEE_ERROR,
                     10, 300);

    m_spec->remove(obj);
    size_t q = m_objs.erase(obj->objid());
    if (q != 1)
    {
        return BEE_ERROR;
    }
    return BEE_SUCCESS;
}


//-----------------------------------------------------------------------------
// Get instances
//-----------------------------------------------------------------------------
int b_objtype::allObject(b_objects& objects)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::allObject");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_lock, BEE_ERROR,
                     2, 400);

    objects.clear();
    b_objectMap::iterator iter = m_objs.begin();
    for ( ; iter != m_objs.end(); iter++)
    {
        b_object * obj = (*iter).second;
        objects.push_back(obj);
    }
    return objects.size();
}

//-----------------------------------------------------------------------------
// Map name standard's and user's
//-----------------------------------------------------------------------------
int b_objtype::mapping(TypeNameMapping& mapdata)
{
    TRACE_FUNCTION(TRL_LOW, "b_objtype::mapping");

    m_userName = mapdata.m_typeName;
    NameMap::iterator iter = mapdata.m_mapping.begin();
    for ( ; iter != mapdata.m_mapping.end(); iter++)
    {
        const string& hosts = (*iter).first;
        const string& users = (*iter).second;
        b_attribute * attr = const_cast<b_attribute *>(this->find(hosts));
        if (attr == NULL)
        {
            TRACE_DEBUG((_TX("Undefined addtibute \"%s\"\n"), hosts.c_str()));
            continue; // Ignore undefined name
        }
        attr->userName(users);
    }

    ObjSpec::instance()->entryUserName(m_name, m_userName);

    return 0;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void b_objtype::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n*** Class: %s(%s) ***\n"),
                         m_name.c_str(), m_userName.c_str()));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  current seed-num is %d.\n"),
                         m_seed.value()));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  [ attributes ]\n")));
    b_attributes::const_iterator at_iter = m_attrs.begin();
    for ( ; at_iter != m_attrs.end(); at_iter++)
    {
        const b_attribute * attr = *at_iter;
        attr->dump();
    }

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("  [ objects ]\n")));
    b_objectMap::const_iterator obj_iter = m_objs.begin();
    for ( ; obj_iter != m_objs.end(); obj_iter++)
    {
        const b_object * obj = (*obj_iter).second;
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("    %s\n"), obj->getObjID()));
    }
}

