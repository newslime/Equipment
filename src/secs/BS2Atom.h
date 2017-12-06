// $Id: BS2Atom.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Atom.h
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

#ifndef BS2ATOM_H
#define BS2ATOM_H

#include "beesecs.h"
#include "BS2value.h"

class BS2id;
class BS2value;
class BS2IStream;
class BS2OStream;
class BS2InterpBase;
class BS2Atom;
struct BS2ItemHeader;

typedef vector<BS2Atom *> BS2Atoms;

/* ------------------------------------------------------------------------- */
class BEE_Export BS2Atom : public BS2value
{
public:
	
    BS2Atom() {
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Atom::BS2Atom");
        }
    BS2Atom(const BS2Atom& rhs);
    BS2Atom(const BS2value& rhs) : BS2value(rhs) {
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Atom::BS2Atom");
        }
    const BS2Atom& operator=(const BS2Atom& rhs);
    const BS2Atom& operator=(const BS2value& rhs);
    virtual ~BS2Atom() {
            TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Atom::~BS2Atom");
        }
    virtual void dump() const = 0;
    virtual void print(BS2InterpBase * interp) const = 0;
    virtual void get(BS2OStream& buf) const = 0;
    virtual void set(BS2IStream& buf) = 0;
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf) = 0;
    virtual BS2Atom * factory(BYTE * data, size_t len) const = 0;
    virtual BS2Atom * replicate() const = 0;

    bool isArray() const   {
            return (m_q > 1 &&
                    m_t != ATOM_ASCII && m_t != ATOM_BINARY && m_t != ATOM_LIST);
       }
    int  format() const    { return ((this->isArray()) ? m_t | ATOM_ARRAY :
                                                         m_t); }
    bool isInt1() const    { return (m_t == ATOM_INT1); }
    bool isInt2() const    { return (m_t == ATOM_INT2); }
    bool isInt4() const    { return (m_t == ATOM_INT4); }
    bool isInt8() const    { return (m_t == ATOM_INT8); }
    bool isUInt1() const   { return (m_t == ATOM_UINT1); }
    bool isUInt2() const   { return (m_t == ATOM_UINT2); }
    bool isUInt4() const   { return (m_t == ATOM_UINT4); }
    bool isUInt8() const   { return (m_t == ATOM_UINT8); }
    bool isList() const    { return (m_t == ATOM_LIST); }
    bool isBinary() const  { return (m_t == ATOM_BINARY); }
    bool isBoolean() const { return (m_t == ATOM_BOOLEAN); }
    bool isAscii() const   { return (m_t == ATOM_ASCII); }
    bool isJis() const     { return (m_t == ATOM_JIS); }
    bool isUnicode() const { return (m_t == ATOM_UNICODE); }
    bool isFloat4() const  { return (m_t == ATOM_FLOAT4); }
    bool isFloat8() const  { return (m_t == ATOM_FLOAT8); }

    bool isInt1Array() const { return (m_q > 1 && m_t == ATOM_INT1); }
    bool isInt2Array() const { return (m_q > 1 && m_t == ATOM_INT2); }
    bool isInt4Array() const { return (m_q > 1 && m_t == ATOM_INT4); }
    bool isInt8Array() const { return (m_q > 1 && m_t == ATOM_INT8); }
    bool isUInt1Array() const { return (m_q > 1 && m_t == ATOM_UINT1); }
    bool isUInt2Array() const { return (m_q > 1 && m_t == ATOM_UINT2); }
    bool isUInt4Array() const { return (m_q > 1 && m_t == ATOM_UINT4); }
    bool isUInt8Array() const { return (m_q > 1 && m_t == ATOM_UINT8); }
    bool isFloat4Array() const { return (m_q > 1 && m_t == ATOM_FLOAT4); }
    bool isFloat8Array() const { return (m_q > 1 && m_t == ATOM_FLOAT8); }
    bool isInt() {
            return (this->isInt4() || this->isInt2() ||
                    this->isInt1() || this->isInt8());
        }
    bool isUInt() {
            return (this->isUInt4() || this->isUInt2() ||
                    this->isUInt1() || this->isUInt8());
        }
    bool isFloat() { return (this->isFloat4() || this->isFloat8()); }

    inline bool haveData() const { return (m_q > 0); }
    inline BS2value& getValue()  { return *this; }

    //
    int get(b_value& val);
    int get(b_id& id);
    //
    static BS2Atom * factory(const BS2ItemHeader& itemHeader);
    static BS2Atom * factory(const BS2Atom * atom);
    static BS2Atom * factory(const b_value& val);
    static BS2Atom * factory(const b_id& id);


protected:
    void copy(const BS2value& rhs);
    void init(int t, int q, int s) {
            m_t = t; m_q = q; m_sz = s;
        }

};

/* ------------------------------------------------------------------------- */

#endif /* BS2ATOM_H */
