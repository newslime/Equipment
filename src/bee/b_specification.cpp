// $Id: b_specification.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_specification.cpp
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

#include "b_specification.h"
#include "b_objtype.h"
#include "b_object.h"


//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_specification::~b_specification()
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::~b_specification");

    this->clear();

}

//-----------------------------------------------------------------------------
void b_specification::clear()
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::clear");

    //
    // Erase Objects
    //
    b_objectMap::iterator obj_iter = m_objs.begin();
    for ( ; obj_iter != m_objs.end(); obj_iter++)
    {
        b_object * _object = (*obj_iter).second;
        delete _object;
    }
    m_objs.clear();

    //
    // Erase ObjTypes
    //
    b_objtypeMap::iterator iter = m_classes.begin();
    for ( ; iter != m_classes.end(); iter++)
    {
        //@ b_objtype * objtype = (*iter).second;
        //@ delete objtype;     // can't delete static objtype
    }
    m_classes.clear();

}

//-----------------------------------------------------------------------------
// Make object specification name
//-----------------------------------------------------------------------------
void b_specification::makeObjSpec(b_object * obj, string& spec) const
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::makeObjSpec");

    //
    // if a parent is supported, make full path name
    //
    spec = m_name;
    if (m_name.size() > 0)
    {
        spec += _TX(">");
    }
    spec += obj->objid();
    return ;
}

//-----------------------------------------------------------------------------
// Get object id name
//-----------------------------------------------------------------------------
int b_specification::objSpec2ID(const string& spec, string& objid) const
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::objSpec2ID");

    //
    // if a parent is supported, make full path name
    //
    int diffpos = strspn(spec.c_str(), m_name.c_str());
    if (diffpos == 0)
    {
        return BEE_ERROR;
    }
    if (spec.at(diffpos) != '>')
    {
        return BEE_ERROR;
    }

    objid = spec.substr(diffpos + 1);
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Find class by name(ObjType)
//-----------------------------------------------------------------------------
b_objtype * b_specification::find(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::find");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon,
                     const_cast<ACE_Thread_Mutex&>(this->m_typeLock), NULL,
                     2, 200);

    b_objtypeMap::const_iterator iter = m_classes.find(name);
    if (iter == m_classes.end())
    {
        return NULL;
    }
    const b_objtype * result = (*iter).second;
    return (b_objtype *)result;
}

//-----------------------------------------------------------------------------
b_objtype * b_specification::findByUserName(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::findByUserName");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon,
                     const_cast<ACE_Thread_Mutex&>(this->m_typeLock), NULL,
                     2, 200);

    string stdname;
    bool result = const_cast<b_specification *>(this)->standardByUserName(name, stdname);
    if (! result)
    {
        return NULL;
    }
    return this->find(stdname);
}

//-----------------------------------------------------------------------------
b_object * b_specification::findObject(const string& name) const
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::findObject");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon,
                     const_cast<ACE_Thread_Mutex&>(this->m_objLock), NULL,
                     2, 400);

    b_objectMap::const_iterator iter = m_objs.find(name);
    if (iter == m_objs.end())
    {
        return NULL;
    }
    const b_object * result = (*iter).second;
    return (b_object *)result;
}

//-----------------------------------------------------------------------------
// Add new class
//-----------------------------------------------------------------------------
int b_specification::add(b_objtype * cls)
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::add");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_typeLock, 1,
                     10, 200);

    m_classes.insert(b_objtypePair(cls->typeName(), cls));

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Entry new object
//-----------------------------------------------------------------------------
int b_specification::addObject(b_object * obj)
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::addObject");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_objLock, 1,
                     10, 200);

    string spec;
    makeObjSpec(obj, spec);
    m_objs.insert(b_objectPair(spec, obj));

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Remove object
//-----------------------------------------------------------------------------
int b_specification::remove(b_object * obj)
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::remove");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_objLock, BEE_SUCCESS+1,
                     10, 300);

    string spec;
    makeObjSpec(obj, spec);
    size_t q = m_objs.erase(spec);
    if (q != 1)
    {
        return BEE_ERROR;
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Remove object type
//-----------------------------------------------------------------------------
int b_specification::remove(b_objtype * cls)
{
    TRACE_FUNCTION(TRL_LOW, "b_specification::remove");

    BEE_GUARD_RETURN(ACE_Thread_Mutex, ace_mon, this->m_typeLock, BEE_SUCCESS+1,
                     10, 300);

    size_t q = m_classes.erase(cls->typeName());
    if (q != 1)
    {
        return BEE_ERROR;
    }
    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// Find class by name(ObjType)
//-----------------------------------------------------------------------------
void b_specification::dump() const
{

    b_objtypeMap::const_iterator iter = m_classes.begin();
    for ( ; iter != m_classes.end(); iter++)
    {
        const b_objtype * klass = (*iter).second;
        klass->dump();
    }
}
