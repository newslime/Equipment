// $Id: BS2Boolean.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Boolean.h
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

#ifndef BS2BOOLEAN_H
#define BS2BOOLEAN_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Boolean : public BS2Atom
{
public:
    BS2Boolean()           { initNull(); }
    BS2Boolean(bool datum) { initv(datum); }
    BS2Boolean(BYTE * data, size_t len = sizeof(bool));
    BS2Boolean(const BS2Boolean& rhs);
    virtual ~BS2Boolean() {}
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Boolean& operator=(const BS2Boolean& rhs);
    const BS2Boolean& operator=(bool rhs);
    bool value() const { return m._bo; }

friend BS2IStream& operator>>(BS2IStream& is, BS2Boolean& atom);
friend BS2OStream& operator<<(BS2OStream& os, const BS2Boolean& atom);

protected:
    void initv(bool b) {
            init(ATOM_BOOLEAN, 1, sizeof(bool)); m._bo = b;
        }
    void initNull() {
            init(ATOM_BOOLEAN, 0, sizeof(bool)); m._bo = false;
        }
};

#endif /* BS2BOOLEAN_H */
