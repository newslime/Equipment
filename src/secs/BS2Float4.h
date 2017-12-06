// $Id: BS2Float4.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Float4.h
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

#ifndef BS2FLOAT4_H
#define BS2FLOAT4_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Float4 : public BS2Atom
{
public:
    BS2Float4()            { initNull(); }
    BS2Float4(float datum) { initv(datum); }
    BS2Float4(BYTE * data, size_t len);
    BS2Float4(const BS2Float4& rhs);
    virtual ~BS2Float4() {}
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Float4& operator=(const BS2Float4& rhs);
    const BS2Float4& operator=(const float data);
    float value() const { return m._fl; }

friend BS2IStream& operator>>(BS2IStream& is, BS2Float4& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2Float4& atom);

protected:
    void initv(float v) {
            init(ATOM_FLOAT4, 1, sizeof(float)); m._fl = v;
        }
    void initNull() {
            init(ATOM_FLOAT4, 0, sizeof(float)); m._fl = 0.0;
        }
};


#endif /* BS2FLOAT4_H */
