// $Id: BS2id.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2id.h
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

#ifndef BS2ID_H
#define BS2ID_H

#include "../bee/b_id.h"
#include "beesecs.h"

class BS2Atom;

class BEE_Export BS2id : public b_id
{
public:
    BS2id():         b_id()   {}
    BS2id(int id):   b_id(id) {}
    BS2id(u_int id): b_id(id) {}
    BS2id(const string& id): b_id(id) {}
    BS2id(const BCHAR * id):  b_id(id) {}
    BS2id(const b_id& rhs):  b_id(rhs) {}
    BS2id(const BS2id& rhs): b_id((const b_id&)rhs) {}
    BS2id(const BS2Atom& atom) { this->copy(atom); }
    ~BS2id() {}

    const BS2id& operator=(const BS2id& rhs) {
            if (this == &rhs)
                return *this;
            this->b_id::operator=((const b_id&)rhs);
            return *this;
        }

    const BS2id& operator=(const BS2Atom& atom) {
            this->copy(atom); return *this;
        }
    BS2Atom * getAtom() const;

    friend bool operator==(const BS2id& lhs, const BS2id& rhs);
    friend bool operator!=(const BS2id& lhs, const BS2id& rhs) {
            return (! (lhs == rhs));
        }
    friend bool operator<(const BS2id& lhs, const BS2id& rhs);
//
protected:
    void copy(const BS2Atom& rhs);
};

//------------------------------------------------------------------------------
inline bool operator==(const BS2id& lhs, const BS2id& rhs)
{
    return ((const b_id&)lhs == (const b_id&)rhs);
}

inline bool operator<(const BS2id& lhs, const BS2id& rhs)
{
    return ((const b_id&)lhs < (const b_id&)rhs);
    return false;
}

typedef vector<BS2id>  BS2ids;
typedef list<BS2id>    BS2idList;
struct bsid_less
{
     bool operator() (const BS2id& x, const BS2id& y) const { return (x < y); }
};

#endif  /* BS2ID_H */
