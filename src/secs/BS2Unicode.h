// $Id: BS2Unicode.h,v 1.7 2004/06/27 08:01:56 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Unicode.h
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


#ifndef BS2UNICODE_H
#define BS2UNICODE_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

#ifndef _MSC_VER
typedef basic_string<wchar_t> wstring;
#endif

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Unicode : public BS2Atom
{
public:
    BS2Unicode()                  { initNull(); }
    BS2Unicode(const string& str) { initv(str); }
    BS2Unicode(const BYTE * data, size_t len);
    BS2Unicode(const BS2Unicode& rhs);
    virtual ~BS2Unicode() { }
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Unicode& operator=(const BS2Unicode& rhs);
    const string& value() const { return *(m._str); }

friend BS2IStream& operator>>(BS2IStream& is, BS2Unicode& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Unicode& atom);

protected:
    void initv(const string& v) {
            init(ATOM_UNICODE, v.size(), sizeof(BCHAR)); m._str = new string(v);
        }
    void initv(const BCHAR * v, size_t len = 0) {
            size_t q = (len == 0) ? _tcslen(v) : len;
            init(ATOM_UNICODE, q, sizeof(BCHAR));
            m._str = new string(v, q);
        }
    void initNull() {
            init(ATOM_UNICODE, 0, sizeof(BCHAR)); m._str = NULL;
        }
};

#endif /* _BS2UNICODE_H */
