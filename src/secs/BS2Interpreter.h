// $Id: BS2Interpreter.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Interpreter.h
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

#ifndef BS2INTERPRETER_H
#define BS2INTERPRETER_H

#include <string>
using namespace std;

class BS2InterpBase
{
//
public:
    virtual int printf(const BCHAR * fmt, ...) = 0;
    virtual int print(const string& msg) = 0;

    void logon()   { m_logging = true; }
    void logoff()  { m_logging = false; }
    bool logging() const { return m_logging; }
    void set_tag(bool f)   { m_tag = f; }
    void set_xml(bool f)   { m_tag = f; }
    bool print_tag() const { return m_tag; }
    bool print_xml() const { return m_xml; }

protected:
    BS2InterpBase(): m_logging(false), m_tag(true), m_xml(false) {}
    virtual ~BS2InterpBase() {}
	
protected:
    bool m_logging;
    bool m_tag;
    bool m_xml;
};
template <class T>
class BS2Interpreter : public BS2InterpBase
{
//
public:
    // Get pointer to the Interpreter instance.
    void interp(T * intp) { m_interp = intp; }
    const T * interp() const { return m_interp; }

    virtual int printf(const BCHAR * fmt, ...);
    virtual int print(const string& msg);

    BS2Interpreter(T * interp = NULL) : m_interp(interp) {}
    virtual ~BS2Interpreter() {}
private:
    T * m_interp;
};

struct Tcl_Interp;
struct PyObject;

typedef BS2Interpreter<Tcl_Interp> BS2TclInterp;
typedef BS2Interpreter<PyObject>   BS2PyInterp;   // Not suported, yet.



#endif
