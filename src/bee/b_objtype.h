// $Id: b_objtype.h,v 1.7 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_objtype.h
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

#ifndef B_OBJTYPE_H
#define B_OBJTYPE_H

#include "b_object.h"
#include "b_attribute.h"
#include "b_specification.h"

//
// ObjType class
//
class BEE_Export b_objtype
{
    friend class b_object;

public:
    b_objtype(const string& name, b_specification * spec = NULL);
    virtual ~b_objtype();

    const string& name() const         { return m_name; }
    const BCHAR * typeName() const     { return m_name.c_str(); }
    void  userName(const string& user) { m_userName = user; }
    const string& userName() const     { return m_userName; }
    const BCHAR * charUserName() const { return m_userName.c_str(); }
    int   attrSize() const             { return (int)m_attrs.size(); }
    const b_attributes& attrs() const  { return m_attrs; }
    const b_specification * specification() const { return m_spec; }

    // Add new attribute
    int add(b_attribute * attr);

    // Find attribute by name
    const b_attribute * find(const string& name) const;
    const b_attribute * findByUser(const string& name) const;

    const b_attribute * at(int p) const
    {
        if (p >= m_attrs.size())
            return NULL;
        return m_attrs[p];
    }

    // Create object
    virtual b_object * instance(const BCHAR * name = NULL);

    // Get all objects
    int allObject(b_objects& objects);

    // Find object by name
    b_object * findObject(const string& name);

    // Map name standard's and user's
    int mapping(TypeNameMapping& mapdata);

    void dump() const;

//
protected:
    int makeObjID(string& retid);
    int entry(b_object * obj);
    int remove(b_object * obj);
    int entryAttrName(b_attribute * attr);

    virtual int init() { return 0; }

protected:
    string        m_name;     // ObjType name
    string        m_userName; // ObjType name by user application
    MUTEX_UInt    m_seed;     // ObjID identifier
    b_attributes  m_attrs;    // attribute table
    b_attrMap     m_dict;     // attribute's dictionary
    b_objectMap   m_objs;     // object's dictionary
    b_specification * m_spec; //

    ACE_Thread_Mutex m_lock;
};


#endif  /* B_OBJTYPE_H */
