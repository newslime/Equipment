// $Id: b_attrdata.h,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_attrdata.h
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

#ifndef B_ATTRDATA_H
#define B_ATTRDATA_H

#include "b_value.h"
#include "b_attribute.h"

class b_object;
typedef map<const string, b_attrdata *, str_less,
                          allocator<b_attrdata *> > AttrDataMap;
typedef AttrDataMap::value_type AttrDataPair;


//-----------------------------------------------------------------------------
// Data member of Object
//
class BEE_Export b_attrdata : public b_value
{
public:
    b_attrdata(const b_object * object, const b_attribute * attr);
    b_attrdata(const b_attrdata& rhs) : b_value(rhs) {
            m_attr = rhs.m_attr; m_obj = rhs.m_obj;
        }
    virtual ~b_attrdata() {}

    b_attrdata& operator=(const b_attrdata& rhs) {
            if (this == &rhs)
                return *this;
            this->b_value::copy((b_value&)rhs);
            m_attr = rhs.m_attr; m_obj = rhs.m_obj;
            return *this;
        }

    const b_attribute * attribute() const { return m_attr; }
    const b_object *    object() const    { return m_obj; }
    const b_objtype *   objtype() const;
    int   position() const { return m_attr->position(); }

    const  string& name() const { return m_attr->name(); }
    // size_t size() const         { return m_value.size(); }
    // size_t dataq() const        { return m_value.dataq(); }
    void   dump() const;

    // Get/Set value from attribute
    b_value& get()               { return *this; }
    int      set(b_value& value) { this->b_value::set(value); return BEE_SUCCESS; }

    friend bool operator<(const b_attrdata& ad1, const b_attrdata& ad2);
    friend bool operator==(const b_attrdata& ad1, const b_attrdata& ad2);

protected:
    b_attrdata() : m_attr(NULL), m_obj(NULL) {}

protected:
    b_attribute * m_attr;
    b_object *    m_obj;
};

//
// Compare
//
inline bool operator<(const b_attrdata& ad1, const b_attrdata& ad2)
{
    return (b_value&)ad1 < (b_value&)ad2;
}
//
inline bool operator==(const b_attrdata& ad1, const b_attrdata& ad2)
{
    return (b_value&)ad1 == (b_value&)ad2;
}


#endif  // B_ATTRDATA_H
