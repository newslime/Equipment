// $Id: BS2Int2.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Int2.h
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

#ifndef BS2INT2_H
#define BS2INT2_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Int2 : public BS2Atom
{
public:
    BS2Int2()            { initNull(); }
    BS2Int2(short datum) { initv(datum); }
    BS2Int2(BYTE * data, size_t len = sizeof(short));
    BS2Int2(const BS2Int2& rhs);
    virtual ~BS2Int2() {}
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Int2& operator=(const BS2Int2& rhs);
    const BS2Int2& operator=(short data);
    short value() const { return m._sh; }

friend BS2IStream& operator>>(BS2IStream& is, BS2Int2& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Int2& atom);

protected:
    void initv(short v) {
            init(ATOM_INT2, 1, sizeof(short)); m._sh = v;
        }
    void initNull() {
            init(ATOM_INT2, 0, sizeof(short)); m._sh = 0;
        }
};


#endif /* BS2INT2_H */
