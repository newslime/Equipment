// $Id: b_enumerator.h,v 1.1 2004/06/21 16:27:23 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_enumerator.h
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2004 BEE Co.,Ltd. All rights reserved.
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

#ifndef JYUGEM_BEE_ENUMERATOR_H
#define JYUGEM_BEE_ENUMERATOR_H

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
using namespace std;


//
// Enumerate Type
//
class b_enumerator
{
public:

    // Enumerate element
    class enum_element
    {
        friend class b_enumerator;
    public:
        enum_element(const char * name, int num)
            : m_name(name), m_num(num) {}
        enum_element(int num, const char * name)
            : m_num(num), m_name(name) {}
        enum_element(const enum_element& rhs)
        {
            m_name = rhs.m_name;
            m_num = rhs.m_num;
        }
        enum_element& operator=(const enum_element& rhs)
        {
            if (this == &rhs)
                return *this;
            m_name = rhs.m_name;
            m_num = rhs.m_num;
            return *this;
        }

        int value() const         { return m_num; }
        const char * name() const { return m_name; }

    protected:
        enum_element() : m_name(0), m_num(0) {}

    protected:
        const char * m_name;
        int   m_num;
    };

    //
    typedef map<string, int, std::less<string>, allocator<int> > map_by_name;
    typedef map<int, string, std::less<int>, allocator<string> >
        map_by_int;

    // Constructor
    b_enumerator(const char * name) : m_name(name) {}
    b_enumerator(const char * name, const enum_element * initv) : m_name(name)
    {
        while (initv->m_name == NULL)
        {
            m_named.insert(std::make_pair(initv->m_name, initv->m_num));
            m_numberd.insert(std::make_pair(initv->m_num, initv->m_name));
            initv++;
        }
    }
    b_enumerator(const char *, vector<char *>& initv)
    {
        for (size_t i = 0; i < initv.size(); i++)
        {
            m_named.insert(std::make_pair(initv[i], i));
            m_numberd.insert(std::make_pair(i, initv[i]));
        }
    }

    b_enumerator(const b_enumerator& rhs)
    {
        m_name = rhs.m_name;
        m_named = rhs.m_named;
        m_numberd = rhs.m_numberd;
    }

    // Destructor
    virtual ~b_enumerator() {}

    // Get/Set member variables
    const string& name() const   { return m_name; }
    const char *  name_p() const { return m_name.c_str(); }

    // Add enumerator element
    int add(const string& mbr, int num)
    {
        pair<map_by_name::iterator, bool> mnge_name;
        mnge_name = m_named.insert(std::make_pair(mbr, num));
        if (mnge_name.second == false)
        {   // member exist already
            return -1;
        }
        pair<map_by_int::iterator, bool> mnge_num;
        mnge_num = m_numberd.insert(std::make_pair(num, mbr));
        if (mnge_num.second == false)
        {   // member exist already, erase the member in named map
            m_named.erase(mnge_name.first);
            return -1;
        }
        return 0;
    }

    // Get enum number by name
    bool stringToNum(const string& elm, int& retnum)
    {
        map_by_name::iterator iter = m_named.find(elm);
        if (iter == m_named.end())
        {
            return false;
        }
        retnum = iter->second;
        return true;
    }

    // Get enum name by number
    bool numToString(int num, string& ename)
    {
        map_by_int::iterator iter = m_numberd.find(num);
        if (iter == m_numberd.end())
        {
            return false;
        }
        ename = iter->second;
        return true;
    }


    static b_enumerator * parse(const char * name, const char * str);

    string toString()
    {
        string result = "{";
        bool first = true;
        map_by_int::iterator iter = m_numberd.begin();
        for ( ; iter != m_numberd.end(); iter++)
        {
            int num = iter->first;
            if (first)
                first = false;
            else
                result += ", ";
            char buf[64];
            sprintf(buf, " = %d", num);
            result += iter->second;
            result += buf;
        }
        result += "};";
        return result;
    }

//
protected:
    string         m_name;
    map_by_name    m_named;
    map_by_int     m_numberd;
};



#endif  // JYUGEM_BEE_ENUMERATOR_H
