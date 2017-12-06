// $Id: BS2Ascii.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Ascii.h
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

#ifndef BS2ASCII_H
#define BS2ASCII_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Ascii : public BS2Atom
{
public:
    BS2Ascii() { initNull(); }
    BS2Ascii(const BYTE * data, size_t len);
    BS2Ascii(const BCHAR * str);
    BS2Ascii(const string& str);
    BS2Ascii(const BS2Ascii& rhs);
    virtual ~BS2Ascii() {}
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Ascii& operator=(const BS2Ascii& rhs);
    const BS2Ascii& operator=(const string& rhs);
    const string& value() const { return *(m._str); }

friend BS2IStream& operator>>(BS2IStream& is, BS2Ascii& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Ascii& atom);

protected:
    void initv(const string& v) {
            init(ATOM_ASCII, v.size(), sizeof(BCHAR)); m._str = new string(v);
        }
    void initv(const BCHAR * v) {
            init(ATOM_ASCII, _tcslen(v), sizeof(BCHAR)); m._str = new string(v);
        }
    void initNull() {
            init(ATOM_ASCII, 0, sizeof(BCHAR)); m._str = NULL;
        }
};

#endif /* BS2ASCII_H */
