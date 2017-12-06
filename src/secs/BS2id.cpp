// $Id: BS2id.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2id.cpp
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

#define BEE_BUILD_DLL

#include "beesecs.h"
#include "BS2id.h"
#include "BS2DeclAtoms.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
void BS2id::copy(const BS2Atom& atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2id::copy");

    if (atom.haveData()) {
        if (atom.isAscii()) {
            string s = (ACE_dynamic_cast(const BS2Ascii &, atom)).value();
            *this = s;
        } else if (atom.isUInt4()) {
            u_int v = (ACE_dynamic_cast(const BS2UInt4 &, atom)).value();
            *this = v;
        } else if (atom.isInt4()) {
            int v = (ACE_dynamic_cast(const BS2Int4 &, atom)).value();
            *this = v;
        } else if (atom.isUInt2()) {
            u_int v = (ACE_dynamic_cast(const BS2UInt2 &, atom)).value();
            *this = v;
        } else if (atom.isUInt1()) {
            u_int v = (ACE_dynamic_cast(const BS2UInt1 &, atom)).value();
            *this = v;
        } else if (atom.isInt2()) {
            short s = (ACE_dynamic_cast(const BS2Int2 &, atom)).value();
            int v = s;
            *this = v;
        } else if (atom.isInt1()) {
            BCHAR c = (ACE_dynamic_cast(const BS2Int1 &, atom)).value();
            int v = c;
            *this = v;
        } else if (atom.isBinary()) {
            BYTE * bp = (ACE_dynamic_cast(const BS2Binary &, atom)).value();
            u_int v = (u_int)*bp;
            *this = v;
        } else {
            TRACE_ERROR((_TX("Illegal variable type %s \n"),
                         const_cast<BS2Atom &>(atom).formatName()));
        }
    } else {
        // <null> data
        if (atom.isAscii()) {
            *this = "";
        } else {
            u_int v = 0;
            *this = v;
        }
    }
}

//-----------------------------------------------------------------------------
// Get item data.
//-----------------------------------------------------------------------------
BS2Atom * BS2id::getAtom() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2id::getAtom");
    BS2Atom * atom;
    if (m_t == ATOM_ASCII) {
        atom = new BS2Ascii(*m._str);
    } else if (m_t == ATOM_UINT4) {
        atom = new BS2UInt4(m._ui);
    } else if (m_t == ATOM_INT4) {
        atom = new BS2Int4(m._in);
    } else if (m_t == ATOM_UINT8) {
        atom = new BS2UInt8(m._ull);
    } else if (m_t == ATOM_INT8) {
        atom = new BS2Int8(m._llo);
    } else {
        TRACE_ERROR((_TX("Illegal variable type %o \n"), m_t));
        return NULL;
    }
    return atom;
}

