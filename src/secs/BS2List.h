// $Id: BS2List.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2List.h
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

#ifndef BS2LIST_H
#define BS2LIST_H

#include "BS2Atom.h"

class BS2IStream;
class BS2OStream;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2List : public BS2Atom
{
public:
    BS2List() 
	{
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2List::BS2List");
            initNull();
    }
    BS2List(const BS2List& rhs);
    virtual ~BS2List();
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2List& operator=(const BS2List& rhs);
    void  add(const BS2Atom * val);
    void  add(const BS2value& val);
    BS2Atoms& getList() { return reinterpret_cast<BS2Atoms&>(*m._vec); }

friend BS2IStream& operator>>(BS2IStream& is, BS2List& atom);
friend BS2OStream& operator<<(BS2OStream& os, BS2List& atom);
//
protected:
    void initv(BS2Atoms& v) {
            init(ATOM_LIST, v.size(), sizeof(BS2Atoms));
            BS2Atoms * atoms = new BS2Atoms();
            for (size_t i = 0; i < v.size(); i++)
            {
                BS2Atom * vp = BS2Atom::factory(v[i]);
                atoms->push_back(vp);
            }
            m._vec = (b_valvect *)atoms;
        }
    void initNull() {
            init(ATOM_LIST, 0, sizeof(BS2Atoms));
            m._vec = (b_valvect *)(new BS2Atoms());
        }
};

#endif /* BS2LIST_H */
