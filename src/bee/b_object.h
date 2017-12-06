// $Id: b_object.h,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_object.h
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

#ifndef B_OBJECT_H
#define B_OBJECT_H

#include "b_specification.h"
#include "b_attrdata.h"

class b_objtype;
class b_attribute;
class b_attrdata;
typedef vector<b_attrdata *> AttrDataVect;

//
// Object
//
class BEE_Export b_object
{
    friend class b_objtype;

public:
    virtual ~b_object();
    b_object& operator=(const b_object& rhs);

    virtual int link() { return BEE_SUCCESS; }

    string& name()           { return m_name; }
    const BCHAR * charName() { return m_name.c_str(); }

    const string& objid() const { return m_name; }
    const string& objSpec() const { return m_spec; }
    const BCHAR * getObjSpec() const { return m_spec.c_str(); }
    const BCHAR * getObjID() const   { return m_name.c_str(); }
    b_objtype *   getObjType() const { return m_objtype; }
    AttrDataVect& getDataTable()     { return m_values; }

    // Find attribute by name
    const b_attribute * findAttr(const string& name) const;
    b_attrdata * findData(const string& name) const;
    b_attrdata * findDataByUser(const string& name) const;

    // Set attribute data
    int set(const b_object& rhs);

    // Compare attribute data
    bool isEqual(const b_object& rhs);

    void dump() const;
//
protected:
    b_object() : m_objtype(NULL) {}
    b_object(b_objtype * oss, const BCHAR * name = NULL);
    b_object(const b_object& rhs);
    void copy(const b_object& rhs);
    void makeDataMap();

//
protected:
    string     m_name;
    string     m_spec;
    b_objtype  * m_objtype;
    AttrDataVect m_values;
};


// Declare template
typedef vector<b_object *>  b_objects;

#endif  /* B_OBJECT_H */
