// $Id: b_command.h,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_command.h
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

#ifndef B_COMMAND_H
#define B_COMMAND_H

#include "b_value.h"
#include "beelib.h"

class b_object;

//
// Command parameter
//
class BEE_Export b_param
{
public:
    string  m_pname;
    b_value m_pval;

    b_param(): m_pname(_TX("")), m_pval(0) {}
    b_param(const string& name, const b_value& val): m_pname(name), m_pval(val) {}
    b_param(const b_param& rhs) {
            m_pname = rhs.m_pname; m_pval = rhs.m_pval;
        }
    b_param& operator=(const b_param& rhs) {
            if (this == &rhs)
                return *this;
            m_pname = rhs.m_pname; m_pval = rhs.m_pval;
            return *this;
        }
};

//
// Remote command
//
class BEE_Export b_command
{
public:
    string m_subject;
    string m_to;
    string m_from;
    vector<b_param> m_params;
    b_object * m_obj;

    b_command() : m_subject(_TX("")), m_to(_TX("")), m_from(_TX("")),
                  m_obj(NULL) {}
    b_command(const string& cmd)
             : m_subject(cmd), m_to(_TX("")), m_from(_TX("")), m_obj(NULL) {}
    b_command(const string& cmd, b_object * obj)
             : m_subject(cmd), m_to(_TX("")), m_from(_TX("")), m_obj(obj) {}
    b_command(const string& cmd, const string& dest, const string& from)
             : m_subject(cmd), m_to(dest), m_from(from), m_obj(NULL) {}
    b_command(const string& cmd, b_object * obj, const string& dest,
              const string& from)
             : m_subject(cmd), m_to(dest), m_from(from), m_obj(obj) {}
    b_command(const b_command& rhs) { copy(rhs); }

    b_command& operator=(const b_command& rhs) {
            if (this == &rhs)
                return *this;
            copy(rhs);
            return *this;
        }
    ~b_command() {}

    const string& name() const     { return m_subject; }
    const BCHAR * charName() const { return m_subject.c_str(); }

    void  name(const string& sub)  { m_subject = sub; }
    void  from(const string& addr) { m_from = addr; }
    void  to(const string& toAddr) { m_to = toAddr; }
    const string& from() const     { return m_from; }
    const string& to() const       { return m_to; }

    void add(const string& pname, const b_value& pval) {
            b_param param(pname, pval);
            m_params.push_back(param);
        }

    b_object * getObject()               { return m_obj; }
    void       setObject(b_object * obj) { m_obj = obj; }
    bool getValue(const string& parname, b_value& retval);

protected:
    void copy(const b_command& rhs) {
            m_subject = rhs.m_subject;
            m_to = rhs.m_to;
            m_from = rhs.m_from;
            m_params = rhs.m_params;
            m_obj = rhs.m_obj;
        }
};


#endif  /* B_COMMAND_H */
