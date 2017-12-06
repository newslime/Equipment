// $Id: BS2value.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2value.h
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 1998-2004 BEE Co.,Ltd. All rights reserved.
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

#ifndef BS2VALUE_H
#define BS2VALUE_H

#include "beesecs.h"
#include "../bee/b_value.h"
#include "BS2id.h"

class BS2Atom;
class BS2TimeValue;
class BS2value;

//
// Value class
//
class BEE_Export BS2value : public b_value
{
public:
    // Constructor
    BS2value(): b_value() {}
    BS2value(bool val):   b_value(val) {}
    BS2value(char val):   b_value(val) {}
    BS2value(u_char val): b_value(val) {}
    BS2value(short val):  b_value(val) {}
    BS2value(u_short val):b_value(val) {}
    BS2value(int val):    b_value(val) {}
    BS2value(u_int val):  b_value(val) {}
    BS2value(float val):  b_value(val) {}
    BS2value(double val): b_value(val) {}
    BS2value(BYTE * val, size_t q): b_value(val, q) {}
    BS2value(const BCHAR * val): b_value(val) {}
    BS2value(const string& val): b_value(val) {}
    BS2value(LONGLONG val):  b_value(val) {}
    BS2value(ULONGLONG val): b_value(val) {}
    BS2value(const b_value& rhs): b_value(rhs) {}
    BS2value(const BS2value& rhs) : b_value(rhs) {}
    BS2value(const BS2id& id): b_value((const b_id&)id) {}
    BS2value(const BS2Atom& atom) { copy(atom); }

    ~BS2value() {}

    BS2value& operator=(bool val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(char val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(u_char val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(short val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(u_short val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(int val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(u_int val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(float val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(double val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(LONGLONG val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(ULONGLONG val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(const string& val) { this->b_value::operator=(val); return *this; }
    BS2value& operator=(const BCHAR * val)  { this->b_value::operator=(val); return *this; }
    BS2value& operator=(const b_value& rhs) { this->b_value::copy(rhs); return *this; }
    BS2value& operator=(const BS2value& rhs) {
            if (this == &rhs)
                return *this;
            this->b_value::copy(rhs);
            return *this;
        }
    BS2value& operator=(const BS2id& id) {
            this->b_value::operator=((const b_id&)id);
            return *this;
        }
    BS2value& operator=(BS2Atom& atom) { copy(atom); return *this; }
    BS2value& operator=(BS2TimeValue& tv);

    int  set(const string& valstr, int format) {
            return this->b_value::set(valstr, format);
        }
    void set(BS2value& value)  { this->b_value::set(value); }
    void get(BS2value& retval) { this->b_value::get(retval); }
    void get(string& retstr)   { this->b_value::get(retstr); }
    BS2Atom * getAtom() const;
    BS2Atom * getAtom(int type) const;
    BS2TimeValue getTimeValue() const;

    static BS2value zero;

    friend bool operator==(const BS2value& lhs, const BS2value& rhs);
    friend bool operator!=(const BS2value& lhs, const BS2value& rhs) {
            return (! (lhs == rhs));
        }
    friend bool operator<(const BS2value& lhs, const BS2value& rhs);

//
protected:
    void copy(const BS2Atom& rhs);
};

//-----------------------------------------------------------------------------
typedef vector<BS2value> BS2values;

//-----------------------------------------------------------------------------

inline bool operator==(const BS2value& lhs, const BS2value& rhs)
{
    return ((const b_value&)lhs == (const b_value&)rhs);
}

inline bool operator<(const BS2value& lhs, const BS2value& rhs)
{
    return ((const b_value&)lhs < (const b_value&)rhs);
}

#endif  /* BS2VALUE_H */
