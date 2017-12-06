// $Id: BS2Binary.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Binary.h
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

#ifndef BS2BINARY_H
#define BS2BINARY_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Binary : public BS2Atom
{
public:
    BS2Binary()           { initNull(); }
    BS2Binary(BYTE datum) {
            BYTE * clone = (BYTE *)malloc(1);
            *clone = datum;
            initv(clone, 1);
        }
    BS2Binary(BYTE * data, size_t len = 1) {
            BYTE * clone = (BYTE *)malloc(len);
            memmove(clone, data, len);
            initv(clone, len);
        }
    BS2Binary(const BS2Binary& rhs);
    virtual ~BS2Binary();
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Binary& operator=(const BS2Binary& rhs);
    BYTE * value() const { return m._bin; }
    void add(BS2Binary * nxt);
    void add(BYTE datum);

friend BS2IStream& operator>>(BS2IStream& is, BS2Binary& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Binary& atom);

protected:
    void initv(BYTE * v, size_t q) {
            init(ATOM_BINARY, q, sizeof(BYTE)); m._bin = v;
        }
    void initNull() {
            init(ATOM_FLOAT4, 0, sizeof(BYTE)); m._bin = NULL;
        }
};

#endif /* BS2BINARY_H */
