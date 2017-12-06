// $Id: b_attribute.h,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_attribute.h
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

#ifndef B_ATTRIBUTE_H
#define B_ATTRIBUTE_H

#include "b_value.h"

class b_objtype;
class b_object;
class b_attribute;
class b_attrdata;
class b_struct;
class b_enumerator;

typedef map<const string, b_attribute *, str_less, allocator<b_attribute *> >
        b_attrMap;
typedef b_attrMap::value_type  b_attrPair;
typedef vector<b_attribute *>  b_attributes;

//
// Attribute of Class
//
class BEE_Export b_attribute
{
    friend class b_objtype;

public:
    enum { READONLY = 0, READWRITE = 1 };

public:
    b_attribute(const BCHAR * name, int type, size_t size = 1,
                int mode = READONLY, bool nodel = true);
    b_attribute(b_objtype& cls, const BCHAR * name, int type, size_t size = 1,
                int mode = READONLY, bool nodel = true);
    virtual ~b_attribute();
    virtual void dump() const;

    const  string& name() const     { return m_name; }
    const  BCHAR * charName() const { return m_name.c_str(); }
    const  string& userName() const     { return m_userName; }
    const  BCHAR * charUserName() const { return m_userName.c_str(); }
    int    type() const     { return m_type; }
    void   type(int format) { m_type = format; }
    int    position() const { return m_position; }
    void   position(int po) { m_position = po; }
    size_t size() const     { return m_size; }
    bool   isVector() const { return (m_type == ATOM_VECTOR); }
    bool   isStruct() const { return (m_type == ATOM_STRUCT); }
    void   name(const string& name)     { m_name = name; }
    void   userName(const string& user) { m_userName = user; }
    int    mode() const   { return m_mode; }
    void   mode(int code) { m_mode = code; }
    bool   nodel() const  { return m_nodel; }
    void   nodel(bool tf) { m_nodel = tf; }
    void   enumerator(b_enumerator * en) { m_enumerator = en; }
    b_enumerator * enumerator()          { return m_enumerator; }

    b_attribute * at(int p)
    {
        if ((size_t)p >= m_attrs.size())
            return NULL;
        return m_attrs[p];
    }
    b_value& getInitValue() { return m_init; }

    // Create Instance
    virtual b_attrdata * instance(const b_object * object) const;

    void  setInitVal(const BCHAR * defdata, int format) {
            string inidata = defdata;
            m_init.set(inidata, format);
        }
    void setInitVal(const b_value& val) { m_init = val; }
    virtual const b_value& getInitVal() const { return m_init; }


protected:
    b_attribute() : m_name(_TX("")), m_userName(_TX("")), m_type(ATOM_LIST),
                    m_size(0), m_position(0), m_mode(READONLY), m_init(0),
                    m_nodel(true), m_enumerator(NULL) {}

// Member variables
protected:
    string m_name;
    string m_userName;
    int    m_type;
    int    m_size;
    int    m_position;
    int    m_mode;
    bool   m_nodel;                // static or not
    b_enumerator * m_enumerator;
    b_value m_init;
    b_attributes m_attrs;          // used by b_vector or b_struct
};

//
// Vector (OBSOLETE)
//
class b_vector : public b_attribute
{
public:
    b_vector(const BCHAR * name, b_attribute * attr, size_t q = 0,
             int mode = READONLY, bool nodel = true);
    b_vector(b_objtype& cls, const BCHAR * name, b_attribute * attr,
             size_t q = 0, int mode = READONLY, bool nodel = true);
    b_vector(b_struct& stt, const BCHAR * name, b_attribute * attr,
             size_t q = 0, int mode = READONLY, bool nodel = true);
    virtual ~b_vector() {}

    // Create Instance
    virtual b_attrdata * instance(const b_object * object) const;

    // Get value
    virtual const b_value& getInitVal() const { return m_init; }

    b_attribute * member() const { return m_attrs[0]; }

//
protected:
};

//
// Struct (OBSOLETE)
//
class b_struct : public b_attribute
{
    friend class b_objtype;
    friend class b_object;

public:
    b_struct(const BCHAR * name, int mode = READONLY, bool nodel = true);
    b_struct(b_objtype& cls, const BCHAR * name, int mode = READONLY, bool nodel = true);
    b_struct(b_struct& stt, const BCHAR * name, int mode = READONLY, bool nodel = true);
    virtual ~b_struct() {}

    // Create Instance
    virtual b_attrdata * instance(const b_object * object) const;

    // Get value
    virtual const b_value& getInitVal() const { return m_init; }

    // Add new attribute
    void add(b_attribute * attr);
    void add(b_attribute * attrs[]);

protected:
    b_struct() : b_attribute(_TX(""), ATOM_STRUCT, 0) {}

//
protected:

};


#endif  /* B_ATTRIBUTE_H */
