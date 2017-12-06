// $Id: b_specification.h,v 1.7 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_specification.h
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

#ifndef B_SPECIFICATION_H
#define B_SPECIFICATION_H

#include "b_value.h"
#include "beelib.h"

class b_objtype;
class b_object;
typedef map<const string, b_objtype *, cstr_less,
                                       allocator<b_objtype *> > b_objtypeMap;
typedef map<const string, b_object *,  cstr_less,
                                       allocator<void *> >  b_objectMap;
typedef b_objtypeMap::value_type b_objtypePair;
typedef b_objectMap::value_type  b_objectPair;

//
// Name table to entry map
//
typedef map<const string, const string, str_less, allocator<string> >
        NameMap;
typedef NameMap::value_type NamePair;

struct TypeNameMapping
{
    string  m_typeName;   // User's class name
    NameMap m_mapping;    // Key must be host's name

    TypeNameMapping(const string& userName) : m_typeName(userName) {}
    void add(const string& standard, const string& user) {
            m_mapping.insert(NamePair(standard, user));
        }
};


//
// Specification
//
class BEE_Export b_specification
{
    friend class ACE_Singleton<b_specification, ACE_Null_Mutex>;

public:

    const string& name() const     { return m_name; }
    const BCHAR * charName() const { return m_name.c_str(); }
    int   count() const        { return (int)m_classes.size(); }
    u_int seed()               { return m_seed++; }

    // Add new class
    int add(b_objtype * objtype);

    // Entry new object
    int addObject(b_object * obj);

    // Remove object type
    int remove(b_objtype * clazz);

    // Remove object
    int remove(b_object * obj);

    // Find class and object by name
    b_objtype * find(const string& name) const;
    b_objtype * findByUserName(const string& name) const;
    b_object  * findObject(const string& name) const;

    // Make object specification and identifier
    void makeObjSpec(b_object * obj, string& spec) const;
    int  objSpec2ID(const string& spec, string& objid) const;

    // clear all objects and objtypes
    void clear();

    // Entry typename to translate inner name
    void entryUserName(const string& gem, const string& app) {
            m_appTypeNames.insert(NamePair(app, gem));
        }

    void dump() const;

protected:
    b_specification() : m_name(_TX("")), m_seed(1) { }
    b_specification(const string& name) : m_name(name), m_seed(1) {}
    b_specification(const BCHAR * name) : m_name(name), m_seed(1)  {}
    ~b_specification();

    bool standardByUserName(const string& app, string& standard) {
            NameMap::iterator iter = m_appTypeNames.find(app);
            if (iter == m_appTypeNames.end()) {
                return false;
            }
            standard = (*iter).second;
            return true;
        }

protected:
    string       m_name;
    MUTEX_UInt   m_seed;
    b_objtypeMap m_classes;
    b_objectMap  m_objs;
    NameMap      m_appTypeNames;

    ACE_Thread_Mutex m_typeLock;
    ACE_Thread_Mutex m_objLock;
};

typedef ACE_Singleton<b_specification, ACE_Null_Mutex> ObjSpec;

#endif  /* B_SPECIFICATION_H */
