// $Id: BS2Int8.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Int8.h
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

#ifndef BS2INT8_H
#define BS2INT8_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Int8 : public BS2Atom
{
public:
    BS2Int8()               { initNull(); }
    BS2Int8(LONGLONG datum) { initv(datum); }
    BS2Int8(BYTE * data, size_t len);
    BS2Int8(const BS2Int8& rhs);
    virtual ~BS2Int8() { }
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Int8& operator=(const BS2Int8& rhs);
    const BS2Int8& operator=(LONGLONG data);
    LONGLONG value() const { return m._llo; }

friend BS2IStream& operator>>(BS2IStream& is, BS2Int8& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Int8& atom);

protected:
    void initv(LONGLONG v) {
            init(ATOM_INT8, 1, sizeof(LONGLONG)); m._llo = v;
        }
    void initNull() {
            init(ATOM_INT8, 0, sizeof(LONGLONG)); m._llo = 0;
        }
};

#endif /* BS2INT8_H */
