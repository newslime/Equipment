// $Id: BS2UInt8.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2UInt8.h
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

#ifndef BS2UINT8_H
#define BS2UINT8_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

#if _MSC_VER > 1000
typedef unsigned __int64 ULONGLONG;
#endif

/* ------------------------------------------------------------------------- */
class BEE_Export BS2UInt8 : public BS2Atom
{
public:
    BS2UInt8()                { initNull(); }
    BS2UInt8(ULONGLONG datum) { initv(datum); }
    BS2UInt8(BYTE * data, size_t len);
    BS2UInt8(const BS2UInt8& rhs);
    virtual ~BS2UInt8() { }
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2UInt8& operator=(const BS2UInt8& rhs);
    const BS2UInt8& operator=(ULONGLONG data);
    ULONGLONG value() const { return m._ull; }

friend BS2IStream& operator>>(BS2IStream& is, BS2UInt8& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2UInt8& atom);

protected:
    void initv(ULONGLONG v) {
            init(ATOM_UINT8, 1, sizeof(ULONGLONG)); m._ull = v;
        }
    void initNull() {
            init(ATOM_UINT8, 0, sizeof(ULONGLONG)); m._ull = 0;
        }
};

#endif /* BS2UINT8_H */
