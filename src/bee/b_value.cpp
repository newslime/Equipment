// $Id: b_value.cpp,v 1.10 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_value.cpp
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

#include "b_value.h"
#include "b_id.h"
#include "b_assert.h"

b_value b_value::zero = 0;

typedef int (*v_setfp)(b_value& v, const string& str);
typedef int (*v_getfp)(b_value& v, string& str);

struct b_valbase
{
    BCHAR * m_fmtname;
    BCHAR * m_smlname;
    v_setfp m_setfp;
    v_getfp m_getfp;
    v_getfp m_arrayfp;
};

static int vset_list(b_value& v, const string& str);
static int vset_bin(b_value& v, const string& str);
static int vset_bool(b_value& v, const string& str);
static int vset_ascii(b_value& v, const string& str);
static int vset_jis(b_value& v, const string& str);
static int vset_int8(b_value& v, const string& str);
static int vset_int1(b_value& v, const string& str);
static int vset_int2(b_value& v, const string& str);
static int vset_int4(b_value& v, const string& str);
static int vset_uint8(b_value& v, const string& str);
static int vset_uint1(b_value& v, const string& str);
static int vset_uint2(b_value& v, const string& str);
static int vset_uint4(b_value& v, const string& str);
static int vset_float8(b_value& v, const string& str);
static int vset_float4(b_value& v, const string& str);
static int vget_list(b_value& v, string& str);
static int vget_bin(b_value& v, string& str);
static int vget_bool(b_value& v, string& str);
static int vget_ascii(b_value& v, string& str);
static int vget_jis(b_value& v, string& str);
static int vget_int8(b_value& v, string& str);
static int vget_int1(b_value& v, string& str);
static int vget_int2(b_value& v, string& str);
static int vget_int4(b_value& v, string& str);
static int vget_uint8(b_value& v, string& str);
static int vget_uint1(b_value& v, string& str);
static int vget_uint2(b_value& v, string& str);
static int vget_uint4(b_value& v, string& str);
static int vget_float8(b_value& v, string& str);
static int vget_float4(b_value& v, string& str);

static int get_array(b_value& v, string& str);
static int get_array_bin(b_value& v, string& str);
static int get_array_bool(b_value& v, string& str);
static int get_array_ascii(b_value& v, string& str);
static int get_array_jis(b_value& v, string& str);
static int get_array_int8(b_value& v, string& str);
static int get_array_int1(b_value& v, string& str);
static int get_array_int2(b_value& v, string& str);
static int get_array_int4(b_value& v, string& str);
static int get_array_uint8(b_value& v, string& str);
static int get_array_uint1(b_value& v, string& str);
static int get_array_uint2(b_value& v, string& str);
static int get_array_uint4(b_value& v, string& str);
static int get_array_float8(b_value& v, string& str);
static int get_array_float4(b_value& v, string& str);

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_value::b_value(const b_id& id)
{
    m_t = id.m_t;
    m_q = id.m_q;
    m_sz = id.m_sz;
    if (m_t == ATOM_ASCII)
    {
        if (id.m._str != NULL)
        {
            m._str = new string(*id.m._str);
        }
        else
        {
            m._str = NULL; m_q = 0;
        }
    }
    else if (m_t == ATOM_INT4)
    {
        m._in = id.m._in;
    }
    else /* if (m_t == ATOM_UINT4) */
    {
        m._ui = id.m._ui;
    }
}

//-----------------------------------------------------------------------------
void b_value::copy(const b_value& rhs)
{
    m_t = rhs.m_t;  m_q = rhs.m_q;  m_sz = rhs.m_sz;
    if (m_t == ATOM_ASCII || m_t == ATOM_JIS || m_t == ATOM_UNICODE)
    {
        m._str = (rhs.m._str != NULL) ? new string(*rhs.m._str) : NULL;
    }
    else if (m_t == ATOM_LIST || m_t == ATOM_STRUCT || m_t == ATOM_VECTOR)
    {
        if (rhs.m._vec != NULL)
        {
            m._vec = new b_valvect();
            for (size_t i = 0; i < rhs.m._vec->size(); i++)
            {
                b_value * rhsval = (*(rhs.m._vec))[i];
                m._vec->push_back(new b_value(*rhsval));
            }
        }
        else
        {
            m._vec = NULL;
        }
    }
    else if (m_t == ATOM_BINARY)
    {
        if (rhs.m._bin)
        {
            m._bin = (BYTE *)malloc(m_q);
            memmove(m._bin, rhs.m._bin, m_q);
        }
        else
        {
            m._bin = rhs.m._bin;
        }
    }
    else if (m_t > ATOM_ARRAY)
    {
        if (rhs.m._ptr)
        {
            m._ptr = (void *)malloc(m_q * m_sz);
            memmove(m._ptr, rhs.m._ptr, m_q * m_sz);
        }
        else
        {
            m._ptr = rhs.m._ptr;  // NULL
        }
    }
    else
    {
        memmove(&m, &rhs.m, sizeof(m));
    }
}


//-----------------------------------------------------------------------------
// Copy operator
//-----------------------------------------------------------------------------
b_value& b_value::operator=(const b_id& id)
{
    clean();
    m_t = id.m_t;
    m_q = id.m_q;
    m_sz = id.m_sz;
    if (m_t == ATOM_ASCII)
    {
        m._str = (id.m._str != NULL) ? new string(*id.m._str) :
                                       new string(_TX(""));
    }
    else if (m_t == ATOM_INT4)
    {
        m._in = id.m._in;
    }
    else
    { // ATOM_UINT4
        m._ui = id.m._ui;
    }
    return *this;
}

//-----------------------------------------------------------------------------
// Calcurate byte size in list
//-----------------------------------------------------------------------------
size_t b_value::total() const
{
    BEEAssert(m_t == ATOM_LIST || m_t == ATOM_STRUCT || m_t == ATOM_VECTOR);
    if (m._vec == NULL)
        return 0;
    size_t result = 0;
    for (size_t i = 0; i < m._vec->size(); i++)
    {
        b_value * v = (*m._vec)[i];
        result += v->size();
    }
    return result;
}

//-----------------------------------------------------------------------------
// Set integer to format
//-----------------------------------------------------------------------------
int b_value::setInt(int v)
{
    if (m_t == ATOM_INT4)         { m._in = v;         return 0; }
    else if (m_t == ATOM_UINT4)   { m._ui = (UINT)v;   return 0; }
    else if (m_t == ATOM_INT2)    { m._sh = (short)v;  return 0; }
    else if (m_t == ATOM_INT1)    { m._ch = (char)v;   return 0; }
    else if (m_t == ATOM_UINT2)   { m._us = (USHORT)v; return 0; }
    else if (m_t == ATOM_UINT1)   { m._uc = (BYTE)v;   return 0; }
    else if (m_t == ATOM_BOOLEAN) { m._bo = (v != 0) ? true : false; return 0; }
    else if (m_t == ATOM_FLOAT4)  { m._fl = (float)v;  return 0; }
    else if (m_t == ATOM_FLOAT8)  { m._do = (double)v; return 0; }
    else if (m_t == ATOM_INT8)    { m._llo = (LONGLONG)v;  return 0; }
    else if (m_t == ATOM_UINT8)   { m._ull = (ULONGLONG)v; return 0; }
    else if (m_t == ATOM_ASCII || m_t == ATOM_JIS || m_t == ATOM_UNICODE)
    {
        BCHAR buf[64];
        _stprintf(buf, _TX("%d"), v);
        *(m._str) = buf;
        m_q = m._str->size();
        return 0;
    }
    return -1;
}

//-----------------------------------------------------------------------------
// Set double to the format data
//-----------------------------------------------------------------------------
int b_value::setDouble(double value)
{
    if (m_t == ATOM_FLOAT4)
    {
        m._fl = (float)value;
    }
    else if (m_t == ATOM_FLOAT8)
    {
        m._do = value;
    }
    else if (m_t == ATOM_INT4)
    {
        m._in = (int)value;
    }
    else if (m_t == ATOM_UINT4)
    {
        m._ui = (UINT)value;
    }
    else if (m_t == ATOM_ASCII || m_t == ATOM_JIS || m_t == ATOM_UNICODE)
    {
        BCHAR buf[128];
        sprintf(buf, "%f", value);
        *(m._str) = buf;
        m_q = m._str->size();
    }
    else if (m_t == ATOM_BINARY)
    {
        this->set((BYTE *)&value, sizeof(value));
    }
    else if (m_t == ATOM_BOOLEAN)
    {
        m._bo = (value != 0.0) ? true : false;
    }
    else if (m_t == ATOM_INT1)
    {
        m._ch = (char)value;
    }
    else if (m_t == ATOM_INT2)
    {
        m._sh = (short)value;
    }
    else if (m_t == ATOM_INT8)
    {
        m._llo = (LONGLONG)value;
    }
    else if (m_t == ATOM_UINT1)
    {
        m._uc = (unsigned char)value;
    }
    else if (m_t == ATOM_UINT2)
    {
        m._us = (unsigned short)value;
    }
    else if (m_t == ATOM_UINT8)
    {
        m._ull = (ULONGLONG)value;
    }
    else
    {
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
//
// Constant data of value atribute
//
//-----------------------------------------------------------------------------
b_valbase _val_const[64] =
{
  { _TX("list"),  _TX("L"),  vset_list, vget_list, get_array },         //  0: ATOM_LIST
  { NULL,         NULL,      NULL, NULL, NULL },                        //  1:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  2:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  3:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  4:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  5:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  6:
  { NULL,         NULL,      NULL, NULL, NULL },                        //  7:
  { _TX("binary"), _TX("B"), vset_bin, vget_bin, get_array_bin },       //  8: ATOM_BINARY
  { _TX("boolean"),_TX("BOOLEAN"), vset_bool,vget_bool, get_array_bool },    //  9: ATOM_BOOLEAN
  { NULL,         NULL,      NULL, NULL, NULL },                        // 10:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 11:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 12:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 13:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 14:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 15:
  { _TX("ascii"), _TX("A"), vset_ascii, vget_ascii, get_array_ascii },    // 16: ATOM_ASCII
  { _TX("jis"),   _TX("J"), vset_jis,   vget_jis, get_array_jis },        // 17: ATOM_JIS
  { _TX("utf-8"), _TX("UNICODE"), vset_jis,   vget_jis, get_array_jis },  // 18: ATOM_UNICODE
  { NULL,         NULL,      NULL, NULL, NULL },                        // 19:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 20:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 21:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 22:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 23:
  { _TX("int8"),  _TX("I8"), vset_int8, vget_int8, get_array_int8 },    // 24: ATOM_INT8
  { _TX("int1"),  _TX("I1"), vset_int1, vget_int1, get_array_int1 },    // 25: ATOM_INT1
  { _TX("int2"),  _TX("I2"), vset_int2, vget_int2, get_array_int2 },    // 26: ATOM_INT2
  { NULL,         NULL,      NULL, NULL, NULL },                        // 27:
  { _TX("int4"),  _TX("I4"), vset_int4, vget_int4, get_array_int4 },    // 28: ATOM_INT4
  { NULL,         NULL,      NULL, NULL, NULL },                        // 29:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 30:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 31:
  { _TX("float8"), _TX("F8"), vset_float8, vget_float8, get_array_float8 }, // 32: ATOM_FLOAT8
  { NULL,         NULL,      NULL, NULL, NULL },                        // 33:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 34:
  { NULL,         NULL,      NULL, NULL, NULL },                        // 35:
  { _TX("float4"), _TX("F4"), vset_float4, vget_float4, get_array_float4 }, // 36: ATOM_FLOAT4
  { NULL,         NULL,      NULL, NULL, NULL },                            // 37:
  { NULL,         NULL,      NULL, NULL, NULL },                            // 38:
  { NULL,         NULL,      NULL, NULL, NULL },                            // 39:
  { _TX("uint8"), _TX("U8"), vset_uint8, vget_uint8, get_array_uint8 }, // 40: ATOM_UINT8
  { _TX("uint1"), _TX("U1"), vset_uint1, vget_uint1, get_array_uint1 }, // 41: ATOM_UINT1
  { _TX("uint2"), _TX("U2"), vset_uint2, vget_uint2, get_array_uint2 }, // 42: ATOM_UINT2
  { NULL,         NULL,      NULL, NULL, NULL },                              // 43:
  { _TX("uint4"), _TX("U4"), vset_uint4, vget_uint4, get_array_uint4 }, // 44: ATOM_UINT4
  { NULL,         NULL,      NULL, NULL, NULL },                              // 45:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 46:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 47:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 48:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 49:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 50:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 51:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 52:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 53:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 54:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 55:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 56:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 57:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 58:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 59:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 60:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 61:
  { NULL,         NULL,      NULL, NULL, NULL },                              // 62:
  { NULL,         NULL,      NULL, NULL, NULL },                             // 63:
};

//-----------------------------------------------------------------------------
// Set string value to binary value by format
//-----------------------------------------------------------------------------
int b_value::set(const string& valstr, int format)
{
    int result;
    if (_val_const[(format & 0x3F)].m_setfp != NULL)
    {
        result = _val_const[(format & 0x3F)].m_setfp(*this, valstr);
    }
    else
    {
        ACE_ERROR((LM_ERROR,
                  ACE_TEXT("b_value: Illegal format 0x%x \n"), format));
        result = -1;
    }
    return m_t;
}

//-----------------------------------------------------------------------------
// Get value into string area
//-----------------------------------------------------------------------------
int b_value::get(string& retval) const
{
    int result;
    if (((m_t & ATOM_ARRAY) == 0) &&
        _val_const[(m_t & 0x3F)].m_getfp != NULL)
    {
        result = _val_const[m_t & 0x3F].m_getfp(const_cast<b_value&>(*this),
                                                  retval);
    }
    else if (ATOM_ARRAY < m_t && m_t <= (ATOM_ARRAY | ATOM_UINT4))
    {
        result = _val_const[m_t & 0x3F].m_arrayfp(const_cast<b_value&>(*this),
                                                  retval);
    }
    else if (m_t == ATOM_VECTOR || m_t == ATOM_STRUCT || m_t == ATOM_ARRAY)
    {
        result = _val_const[ATOM_LIST].m_getfp(const_cast<b_value&>(*this),
                                               retval);
    }
    else
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("b_value: Illegal format 0x%x \n"), m_t));
        result = -1;
    }
    return result;
}

//-----------------------------------------------------------------------------
// Get value into string area
//-----------------------------------------------------------------------------
string b_value::toString() const
{
    string str;
    this->get(str);
    return str;
}


//-----------------------------------------------------------------------------
// Get format name (static)
//-----------------------------------------------------------------------------
const BCHAR * b_value::formatStr(int format)
{
    if (format >= 64)
        return NULL;

    BCHAR * name = _val_const[format].m_fmtname;
    return name;
}

//-----------------------------------------------------------------------------
const BCHAR * b_value::smlStr(int format)
{
    if (format >= 64)
        return NULL;

    BCHAR * name = _val_const[format].m_smlname;
    return name;
}

//-----------------------------------------------------------------------------
// Convert format code (static)
//-----------------------------------------------------------------------------
int b_value::strToFormat(const BCHAR * frmptr)
{
    string frmStr(frmptr);
    for (int i = 0; i < 64; i++)
    {
        if (_val_const[i].m_fmtname == NULL)
            continue;

        if (frmStr == _val_const[i].m_fmtname)
        {
            return i;
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------
int b_value::smlToFormat(const BCHAR * smlptr)
{
    string smlStr(smlptr);
    for (int i = 0; i < 64; i++)
    {
        if (_val_const[i].m_fmtname == NULL)
            continue;

        if (smlStr == _val_const[i].m_smlname)
        {
            return i;
        }
    }
    return -1;
}

//-----------------------------------------------------------------------------
//
// Convert string to binary
//
//-----------------------------------------------------------------------------
static int vset_list(b_value& v, const string& str)
{
    b_valvect vv;
    b_value * val = new b_value();
    //
    if (str.size() > 0)
    {
        ACE_ERROR((LM_ERROR,
            ACE_TEXT("b_value::vset_list: Parsing string to list is not implement, yet.\n")));
    }
    //
    vv.push_back(val);
    v = vv;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_bin(b_value& v, const string& str)
{
    vector<BYTE> binvec;
    BYTE val(0);
    //
    string token;
    size_t tpos;
    size_t epos;
    if (str.size() > 0 &&
        ((tpos = str.find_first_of("0123456789")) != string::npos))
    {
        epos = str.find(' ', tpos);
        while (epos != string::npos)
        {
            token = str.substr(tpos, epos - tpos);
            val = (BYTE)_tcstoul(token.c_str(), NULL, 16);
            binvec.push_back(val);
            tpos = str.find_first_of("0123456789", epos);
            if (tpos == string::npos)
                break;
            epos = str.find(' ', tpos);
        }
        if (tpos != string::npos)
        {
            token = str.substr(tpos);
            val = (BYTE)_tcstoul(token.c_str(), NULL, 16);
            binvec.push_back(val);
        }
    }
    else
    {
        binvec.push_back(val);
    }
    BYTE * bin = (BYTE *)malloc(binvec.size());
    for (size_t i = 0; i < binvec.size(); i++)
    {
        *(bin + i) = (BYTE)binvec[i];
    }
    v.set(bin, binvec.size());
    free(bin);

    return 0;
}

//-----------------------------------------------------------------------------
static int vset_bool(b_value& v, const string& str)
{
    bool b = ((str == _TX("true")) || (str == _TX("TRUE"))) ? true : false;
    v = b;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_ascii(b_value& v, const string& str)
{
    v = str;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_jis(b_value& v, const string& str)
{
    v = str;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_int1(b_value& v, const string& str)
{
    char c = (char)_tcstol(str.c_str(), NULL, 0);
    v = c;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_int2(b_value& v, const string& str)
{
    short s = (short)_tcstol(str.c_str(), NULL, 0);
    v = s;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_int4(b_value& v, const string& str)
{
    int i = _tcstol(str.c_str(), NULL, 0);
    v = i;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_int8(b_value& v, const string& str)
{
#ifdef WIN32
    LONGLONG l = _atoi64(str.c_str());
#else
    LONGLONG l = strtoll(str.c_str(), NULL, 0);
#endif
    v = l;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_float4(b_value& v, const string& str)
{
    float f = (float)atof(str.c_str());
    v = f;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_float8(b_value& v, const string& str)
{
    double d = _tcstod(str.c_str(), NULL);
    v = d;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_uint1(b_value& v, const string& str)
{
    u_char u = (u_char)_tcstoul(str.c_str(), NULL, 0);
    v = u;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_uint2(b_value& v, const string& str)
{
    u_short u = (u_short)_tcstoul(str.c_str(), NULL, 0);
    v = u;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_uint4(b_value& v, const string& str)
{
    u_int u = (u_int)_tcstoul(str.c_str(), NULL, 0);
    v = u;
    return 0;
}

//-----------------------------------------------------------------------------
static int vset_uint8(b_value& v, const string& str)
{
#ifdef WIN32
    ULONGLONG u = (ULONGLONG)_strtoui64(str.c_str(), NULL, 0);
#else
    ULONGLONG u = (ULONGLONG)strtoull(str.c_str(), NULL, 0);
#endif
    v = u;
    return 0;
}

//-----------------------------------------------------------------------------
//
// Convert to string
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static int vget_list(b_value& v, string& str)
{
    str = _TX("");
    string vbuf;
    for (size_t i = 0; i < v.m._vec->size(); i++)
    {
        if (i > 0)
            str += _TX(", ");

        (*v.m._vec)[i]->get(vbuf);
        str += vbuf;
    }
    str += _TX(" ");
    return 0;
}

//-----------------------------------------------------------------------------
static int get_array(b_value& v, string& str)
{
    str = _TX("");
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_bin(b_value& v, string& str)
{
    char buf[32];
    str = _TX("");
    BYTE * bptr = v.m._bin;
    for (int i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, _TX("0x%02X"), *(bptr + i));
        str += buf;
    }
    return 0;
}

//
static int get_array_bin(b_value& v, string& str)
{
    str = _TX("");
    return 0;
}


//-----------------------------------------------------------------------------
static int vget_bool(b_value& v, string& str)
{
    // str = (v.m._bo) ? _TX("true") : _TX("false");
    str = (v.m._bo) ? _TX("TRUE") : _TX("FALSE");
    return 0;
}

//
static int get_array_bool(b_value& v, string& str)
{
    str = _TX("");
    bool * vptr = (bool *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        str += (*vptr) ? _TX("TRUE") : _TX("FALSE");
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_ascii(b_value& v, string& str)
{
    str =  (v.m._str) ? *(v.m._str) : _TX("");
    return 0;
}

//
static int get_array_ascii(b_value& v, string& str)
{
    str = _TX("");
    char ** vptr = (char **)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        str += "\"";
        str += *vptr;
        str += "\"";
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_jis(b_value& v, string& str)
{
    str = *(v.m._str);
    return 0;
}

//
static int get_array_jis(b_value& v, string& str)
{
    str = _TX("");
    char ** vptr = (char **)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        str += "\"";
        str += *vptr;
        str += "\"";
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_int1(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%d"), v.m._ch);
    str = buf;
    return 0;
}

//
static int get_array_int1(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    char * vptr = (char *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%d", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_int2(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%d"), v.m._sh);
    str = buf;
    return 0;
}

//
static int get_array_int2(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    short * vptr = (short *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%d", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_int4(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%d"), v.m._in);
    str = buf;
    return 0;
}

//
static int get_array_int4(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    int * vptr = (int *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%d", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_int8(b_value& v, string& str)
{
    char buf[48];
#ifdef WIN32
    _stprintf(buf, _TX("%I64d"), v.m._llo);
#else
    _stprintf(buf, _TX("%lld"), v.m._llo);
#endif
    str = buf;
    return 0;
}

//
static int get_array_int8(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    LONGLONG * vptr = (LONGLONG *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

#ifdef WIN32
        _stprintf(buf, _TX("%I64d"), *vptr);
#else
        _stprintf(buf, _TX("%lld"), *vptr);
#endif
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_float4(b_value& v, string& str)
{
    char buf[64];
    _stprintf(buf, _TX("%f"), (double)v.m._fl);
    size_t p = _tcslen(buf) - 1;
    while (p > 0 && *(buf + p) == '0')
        p--;
    buf[p + 1] = '\0';
    str = buf;
    return 0;
}

//
static int get_array_float4(b_value& v, string& str)
{
    char buf[64];
    str = _TX("");
    float * vptr = (float *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%f", *vptr);
        size_t p = _tcslen(buf) - 1;
        while (p > 0 && *(buf + p) == '0')
            p--;
        buf[p + 1] = '\0';
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_float8(b_value& v, string& str)
{
    char buf[64];
    _stprintf(buf, _TX("%f"), v.m._do);
    size_t p = _tcslen(buf) - 1;
    while (p > 0 && *(buf + p) == '0')
        p--;
    buf[p + 1] = '\0';
    str = buf;
    return 0;
}

//
static int get_array_float8(b_value& v, string& str)
{
    char buf[64];
    str = _TX("");
    double * vptr = (double *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%f", *vptr);
        size_t p = _tcslen(buf) - 1;
        while (p > 0 && *(buf + p) == '0')
            p--;
        buf[p + 1] = '\0';
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_uint1(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%u"), v.m._uc);
    str = buf;
    return 0;
}

//
static int get_array_uint1(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    unsigned char * vptr = (unsigned char *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%u", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_uint2(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%u"), v.m._us);
    str = buf;
    return 0;
}

//
static int get_array_uint2(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    unsigned short * vptr = (unsigned short *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%u", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_uint4(b_value& v, string& str)
{
    char buf[48];
    _stprintf(buf, _TX("%u"), v.m._ui);
    str = buf;
    return 0;
}

//
static int get_array_uint4(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    unsigned int * vptr = (unsigned int *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

        _stprintf(buf, "%u", *vptr);
        str += buf;
        vptr++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
static int vget_uint8(b_value& v, string& str)
{
    char buf[48];
#ifdef WIN32
    _stprintf(buf, _TX("%I64u"), v.m._ull);
#else
    _stprintf(buf, _TX("%llu"), v.m._ull);
#endif
    str = buf;
    return 0;
}

//
static int get_array_uint8(b_value& v, string& str)
{
    char buf[48];
    str = _TX("");
    ULONGLONG * vptr = (ULONGLONG *)v.m._ptr;
    for (size_t i = 0; i < v.m_q; i++)
    {
        if (i > 0)
            str += _TX(" ");

#ifdef WIN32
        _stprintf(buf, _TX("%I64u"), *vptr);
#else
        _stprintf(buf, _TX("%llu"), *vptr);
#endif
        str += buf;
        vptr++;
    }
    return 0;
}

