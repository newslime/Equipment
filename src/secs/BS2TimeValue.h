// $Id: BS2TimeValue.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TimeValue.h
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

#ifndef BS2TIMEVALUE_H
#define BS2TIMEVALUE_H

#include "beesecs.h"


// Define an object to hold configure data
class BEE_Export BS2TimeValue : public ACE_Time_Value
{
public:
    BS2TimeValue() : ACE_Time_Value() {}
    BS2TimeValue(long sec, long usec = 0)  : ACE_Time_Value(sec, usec) {}
    BS2TimeValue(const struct timeval &t)  : ACE_Time_Value(t) {}
    BS2TimeValue(const timespec_t &t)      : ACE_Time_Value(t) {}
    BS2TimeValue(const ACE_Time_Value &tv) : ACE_Time_Value(tv) {}
    BS2TimeValue(const BS2TimeValue &tv)   : ACE_Time_Value(tv) {}
    BS2TimeValue(const string& ts);
    ~BS2TimeValue() {}

#if 0
    BS2TimeValue& operator=(long rhs) {
            this->set(rhs);  return *this;
        }
    BS2TimeValue& operator=(const BS2TimeValue& rhs) {
            if (this == &rhs)
                return *this;
            this->ACE_Time_Value::set((timeval)rhs);
            return *this;
        }
    BS2TimeValue& operator=(const BCHAR * rhs) {
            this->set(rhs);  return *this;
        }
    BS2TimeValue& operator=(const string& rhs) {
            this->set(rhs);  return *this;
        }
#endif

    void set(long sec, long usec = 0)  { this->ACE_Time_Value::set(sec, usec); }
    void set(const struct timeval &t)  { this->ACE_Time_Value::set(t); }
    void set(const timespec_t &t)      { this->ACE_Time_Value::set(t); }
    void set(const string& ts) { this->set(ts.c_str()); }
    void set(const BCHAR * ts);
    void get(string& ts);

    void operator += (const ACE_Time_Value &tv)
        { (ACE_Time_Value&)(*this) += (ACE_Time_Value&)tv; }

    void operator -= (const ACE_Time_Value &tv)
        { (ACE_Time_Value&)(*this) -= (ACE_Time_Value&)tv; }

    BS2TimeValue& operator *= (double d)
        { return (BS2TimeValue&)((ACE_Time_Value&)(*this) *= d); }

    friend BS2TimeValue operator + (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (BS2TimeValue)((ACE_Time_Value&)tv1 + (ACE_Time_Value &)tv2); }

    friend BS2TimeValue operator + (const BS2TimeValue &tv1, const ACE_Time_Value &tv2)
        { return (BS2TimeValue)((ACE_Time_Value&)tv1 + tv2); }

    friend BS2TimeValue operator - (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (BS2TimeValue)((ACE_Time_Value&)tv1 - (ACE_Time_Value &)tv2); }

    friend BS2TimeValue operator - (const BS2TimeValue &tv1, const ACE_Time_Value &tv2)
        { return (BS2TimeValue)((ACE_Time_Value&)tv1 - tv2); }

    friend int operator < (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 < (ACE_Time_Value &)tv2; }

    friend int operator < (const BS2TimeValue &tv1, const ACE_Time_Value &tv2)
        { return (ACE_Time_Value&)tv1 < tv2; }

    friend int operator > (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 > (ACE_Time_Value &)tv2; }

    friend int operator <= (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 <= (ACE_Time_Value &)tv2; }

    friend int operator >= (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 >= (ACE_Time_Value &)tv2; }

    friend int operator == (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 == (ACE_Time_Value &)tv2; }

    friend int operator == (const BS2TimeValue &tv1, const ACE_Time_Value &tv2)
        { return (ACE_Time_Value&)tv1 == tv2; }

    friend int operator != (const BS2TimeValue &tv1, const BS2TimeValue &tv2)
        { return (ACE_Time_Value&)tv1 != (ACE_Time_Value &)tv2; }

    friend int operator != (const BS2TimeValue &tv1, const ACE_Time_Value &tv2)
        { return (ACE_Time_Value&)tv1 != tv2; }

    void dump() const { this->ACE_Time_Value::dump(); }

protected:

};


#endif  /* BS2TIMEVALUE_H */
