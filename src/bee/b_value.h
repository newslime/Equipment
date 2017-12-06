// $Id: b_value.h,v 1.9 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_value.h
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

#ifndef B_VALUE_H
#define B_VALUE_H

#include "b_ace.h"

#ifndef FORMAT_CODE_DEFINED
#define FORMAT_CODE_DEFINED
enum
{
    ATOM_LIST = 000,
    ATOM_BINARY = 010,
    ATOM_BOOLEAN = 011,
    ATOM_ASCII = 020,
    ATOM_JIS = 021,
    ATOM_UNICODE = 022,
    ATOM_INT8 = 030,
    ATOM_INT1 = 031,
    ATOM_INT2 = 032,
    ATOM_INT4 = 034,
    ATOM_FLOAT8 = 040,
    ATOM_FLOAT4 = 044,
    ATOM_UINT8 = 050,
    ATOM_UINT1 = 051,
    ATOM_UINT2 = 052,
    ATOM_UINT4 = 054,
    ATOM_STRUCT = 070,
    ATOM_ARRAY = 0100,
    ATOM_VECTOR = 0170,
};
#endif

class b_id;
class b_value;
typedef vector<b_value *> b_valvect;

//
// Value class
//
class BEE_Export b_value
{
public:
    short  m_t;                 // type
    short  m_sz;                // byte size
    size_t m_q;                 // quantity
    union {                     // value
        bool        _bo;
        char        _ch;
        BYTE        _uc;
        int         _in;
        UINT        _ui;
        short       _sh;
        USHORT      _us;
        float       _fl;
        double      _do;
        LONGLONG    _llo;
        ULONGLONG   _ull;
        string *    _str;
        BYTE *      _bin;
        b_valvect * _vec;
        void *      _ptr;
    } m;

    // Constructor
    b_value(): m_t(ATOM_INT4), m_sz(0), m_q(0) { m._in = 0;  }
    b_value(bool val): m_t(ATOM_BOOLEAN), m_sz(sizeof(bool)), m_q(1) {
            m._bo = val;
        }
    b_value(char val): m_t(ATOM_INT1), m_sz(sizeof(char)), m_q(1) {
            m._ch = val;
        }
    b_value(BYTE val): m_t(ATOM_UINT1), m_sz(sizeof(BYTE)), m_q(1) {
            m._uc = val;
        }
    b_value(short val): m_t(ATOM_INT2), m_sz(sizeof(short)), m_q(1) {
            m._sh = val;
        }
    b_value(USHORT val): m_t(ATOM_UINT2), m_sz(sizeof(USHORT)), m_q(1) {
            m._us = val;
        }
    b_value(int val): m_t(ATOM_INT4), m_sz(sizeof(int)), m_q(1) {
            m._in = val;
        }
    b_value(UINT val): m_t(ATOM_UINT4), m_sz(sizeof(UINT)), m_q(1) {
            m._ui = val;
        }
    b_value(float val): m_t(ATOM_FLOAT4), m_sz(sizeof(float)), m_q(1) {
            m._fl = val;
        }
    b_value(double val): m_t(ATOM_FLOAT8), m_sz(sizeof(double)), m_q(1) {
            m._do = val;
        }
    b_value(BYTE * val, size_t q): m_t(ATOM_BINARY), m_sz(sizeof(BYTE)), m_q(q) {
            if (val != NULL && q > 0) {
                m._bin = (BYTE *)malloc(q);
                memmove(m._bin, val, q);
            } else {
                m._bin = NULL;
            }
        }
    b_value(const BCHAR * val): m_t(ATOM_ASCII), m_sz(sizeof(BCHAR)) {
            if (val != NULL) {
                m._str = new string(val);
                m_q = m._str->size();
            } else {
                m._str = NULL;
                m_q = 0;
            }
        }
    b_value(const string& val): m_t(ATOM_ASCII), m_sz(sizeof(BCHAR)) {
            m._str = (val.size() > 0) ? new string(val) : NULL;
            m_q = val.size();
        }
    b_value(LONGLONG val): m_t(ATOM_INT8), m_sz(sizeof(LONGLONG)), m_q(1) {
            m._llo = val;
        }
    b_value(ULONGLONG val): m_t(ATOM_UINT8), m_sz(sizeof(ULONGLONG)), m_q(1) {
            m._ull = val;
        }
    b_value(const b_valvect& val): m_t(ATOM_LIST), m_sz(0) {
            m._vec = (val.size() > 0) ? new b_valvect(val) : new b_valvect();
            m_q = val.size();
        }
    b_value(b_valvect * any, int type): m_t(type), m_sz(0) {  // type is STRUCT or VECTOR
            m._vec = any;
            m_q = (any != NULL) ? any->size() : 0;
        }
    b_value(const b_value& rhs) { copy(rhs); }
    b_value(const b_id& id);

    // Destructor
    inline void clean() {
            if (m_t == ATOM_ASCII && m._str != NULL) {
                delete m._str;  m._str = NULL;
            } else if (m_t == ATOM_LIST && m._vec != NULL) {
                delete m._vec;  m._vec = NULL;
            } else if (m_t == ATOM_BINARY && m._bin != NULL) {
                free(m._bin);  m._bin = NULL;
            } else if ((m_t == ATOM_STRUCT || m_t == ATOM_VECTOR) &&
                        m_q > 0 && m._vec != NULL) {
                delete m._vec;  m._vec = NULL;
            } else if ((m_t & ATOM_ARRAY) && m_q > 0 && m._ptr != NULL) {
                free(m._ptr);  m._ptr = NULL;
            } else if ((m_t == ATOM_JIS || m_t == ATOM_UNICODE) &&
                        m._str != NULL) {
                delete m._str;  m._str = NULL;
            }
        }
    virtual ~b_value() { clean(); }

    b_value& operator=(bool val) {
            clean();
            m_t = ATOM_BOOLEAN; m_q = 1; m_sz = sizeof(bool);
            m._bo = val; return *this;
        }
    b_value& operator=(char val) {
            clean();
            m_t = ATOM_INT1; m_q = 1; m_sz = sizeof(char);
            m._ch = val; return *this;
        }
    b_value& operator=(BYTE val) {
            clean();
            m_t = ATOM_UINT1; m_q = 1; m_sz = sizeof(BYTE);
            m._uc = val; return *this;
        }
    b_value& operator=(short val) {
            clean();
            m_t = ATOM_INT2; m_q = 1; m_sz = sizeof(short);
            m._sh = val; return *this;
        }
    b_value& operator=(USHORT val) {
            clean();
            m_t = ATOM_UINT2; m_q = 1; m_sz = sizeof(USHORT);
            m._us = val; return *this;
        }
    b_value& operator=(int val) {
            clean();
            m_t = ATOM_INT4; m_q = 1; m_sz = sizeof(int);
            m._in = val; return *this;
        }
    b_value& operator=(UINT val) {
            clean();
            m_t = ATOM_UINT4; m_q = 1; m_sz = sizeof(UINT);
            m._ui = val; return *this;
        }
    b_value& operator=(float val) {
            clean();
            m_t = ATOM_FLOAT4; m_q = 1; m_sz = sizeof(float);
            m._fl = val; return *this;
        }
    b_value& operator=(double val) {
            clean();
            m_t = ATOM_FLOAT8; m_q = 1; m_sz = sizeof(double);
            m._do = val; return *this;
        }
    b_value& operator=(const string& val) {
            clean();
            m_t = ATOM_ASCII; m_q = val.size(); m_sz = sizeof(char);
            m._str = (val.size() > 0) ? new string(val) : NULL;
            return *this;
        }
    b_value& operator=(LONGLONG val) {
            clean();
            m_t = ATOM_INT8; m_q = 1; m_sz = sizeof(LONGLONG);
            m._llo = val; return *this;
        }
    b_value& operator=(ULONGLONG val) {
            clean();
            m_t = ATOM_UINT8; m_q = 1; m_sz = sizeof(ULONGLONG);
            m._ull = val; return *this;
        }
    b_value& operator=(const BCHAR * val) {
            clean();
            m_t = ATOM_ASCII;  m_q = (val != NULL) ? strlen(val) : 0;
            m_sz = sizeof(BCHAR);
            m._str = (val != NULL) ? new string(val) : NULL;
            return *this;
        }
    b_value& operator=(const b_valvect& val) {
            clean();
            m_t = ATOM_LIST; m_q = val.size(); m_sz = 0;
            m._vec = (val.size() > (size_t)0) ? new b_valvect(val) :
                                                new b_valvect();
            return *this;
        }
    b_value& operator=(const b_value& rhs) {
            if (this == &rhs)
                return *this;
            clean();
            copy(rhs);
            return *this;
        }
    b_value& operator=(const b_id& id);

    inline int  format() const { return m_t; }
    inline void format(int t)  { m_t = t; }
    size_t size() const {
            if (m_t == ATOM_LIST || m_t == ATOM_STRUCT || m_t == ATOM_VECTOR)
                return total();
            else if (m_t == ATOM_ASCII || m_t == ATOM_JIS || m_t == ATOM_UNICODE)
                return ((m._str) ? m._str->size() + 1 : 0);
            return (m_sz * m_q);
        }
    size_t dataq() const {
            if (m_t == ATOM_LIST || m_t == ATOM_STRUCT || m_t == ATOM_VECTOR) {
                size_t q = 0;
                for (int i = 0; i < (int)m._vec->size(); i++) {
                    b_value * valptr = (*m._vec)[i];
                    q += valptr->dataq();
                }
                return (q + 1);    // Add own
            } else {
                return 1;
            }
        }
    const BCHAR * formatName() { return b_value::formatStr(m_t); }
    const BCHAR * smlName()    { return b_value::smlStr(m_t); }
    bool isEmpty() const       { return (m_q == 0); }
    bool isNil() const {
            if (m_q == 0) {
                return true;
            } else if (m_t == ATOM_ASCII) {
                return (m._str->size() == 0);
            } else if (m_t == ATOM_UINT4) {
                return (m._ui == 0);
            } else if (m_t == ATOM_INT4) {
                return (m._in == 0);
            } else if (m_t == ATOM_UINT2) {
                return (m._us == 0);
            } else if (m_t == ATOM_INT2) {
                return (m._sh == 0);
            } else if (m_t == ATOM_UINT1) {
                return (m._uc == 0);
            } else if (m_t == ATOM_INT1) {
                return (m._ch == 0);
            } else if (m_t == ATOM_FLOAT4) {
                return (m._fl == 0.0);
            } else if (m_t == ATOM_FLOAT8) {
                return (m._do == 0.0);
            } else if (m_t == ATOM_INT8) {
                return (m._llo == 0);
            } else if (m_t == ATOM_UINT8) {
                return (m._ull == 0);
            } else if (m_t == ATOM_JIS || m_t == ATOM_UNICODE) {
                return (m._str->size() == 0);
            } else if (m_t == ATOM_LIST || m_t == ATOM_STRUCT ||
                       m_t == ATOM_VECTOR) {
                if (m._vec == NULL)
                    return true;
                return (m._vec->size() == 0);
            } else /* if (lhs.m_t == ATOM_BOOLEAN) */ {
                return false;
            }
        }
    bool getBool() const {
            if (m_t == ATOM_BOOLEAN)     { return m._bo; }
            else if (m_t == ATOM_INT4)   { return (m._in != 0); }
            else if (m_t == ATOM_UINT4)  { return (m._ui != 0); }
            else if (m_t == ATOM_INT2)   { return (m._sh != 0); }
            else if (m_t == ATOM_UINT2)  { return (m._us != 0); }
            else if (m_t == ATOM_INT1)   { return (m._ch != 0); }
            else if (m_t == ATOM_UINT1)  { return (m._uc != 0); }
            else if (m_t == ATOM_INT8)   { return (m._llo != 0L); }
            else if (m_t == ATOM_UINT8)  { return (m._ull != 0L); }
            else if (m_t == ATOM_FLOAT4) { return (m._fl != 0.0); }
            else if (m_t == ATOM_FLOAT8) { return (m._do != 0.0); }
            return false;
        }
	
	float getfloat() const
	{
		return m._fl; 
	}

    int getInt() const 
	{
        if (m_t == ATOM_INT4) 
		{ 
			return m._in; 
		}
        else if (m_t == ATOM_UINT4) 
		{ 
			return (int)m._ui; 
		}
        else if (m_t == ATOM_INT2)  
		{ 
			return (int)m._sh; 
		}
        else if (m_t == ATOM_INT1)  
		{ 
			return (int)m._ch; 
		}
        else if (m_t == ATOM_UINT2) 
		{ 
			return (int)m._us; 
		}
        else if (m_t == ATOM_UINT1) 
		{ 
			return (int)m._uc; 
		}
        else if (m_t == ATOM_INT8)  
		{ 
			return (int)m._llo; 
		}
        else if (m_t == ATOM_UINT8) 
		{ 
			return (int)m._ull; 
		}
        else if (m_t == ATOM_BOOLEAN) 
		{ 
			return (m._bo) ? 1 : 0; 
		}
        else if (m_t == ATOM_FLOAT4)  
		{ 
			long l = (long)m._fl; 
			return (int)l; 
		}
        else if (m_t == ATOM_FLOAT8)  
		{ 
			long l = (long)m._do; 
			return (int)l; 
		}
        else if (m_t == ATOM_BINARY)  
		{
            if (m._bin != NULL && m_q == 1) 
			{
                int i = *(m._bin);
                return i;
            }
        }
        
		return 0;
	}
    
	LONGLONG getInt2() const 
	{
        if (m_t == ATOM_INT8)		{ return m._llo; }
        else if (m_t == ATOM_INT4)	{ return (LONGLONG)m._in; }
        else if (m_t == ATOM_UINT4) { return (LONGLONG)m._ui; }
        else if (m_t == ATOM_INT2)  { return (LONGLONG)m._sh; }
        else if (m_t == ATOM_INT1)  { return (LONGLONG)m._ch; }
        else if (m_t == ATOM_UINT2) { return (LONGLONG)m._us; }
        else if (m_t == ATOM_UINT1) { return (LONGLONG)m._uc; }
        else if (m_t == ATOM_UINT8) { return (LONGLONG)m._ull; }
        else if (m_t == ATOM_BOOLEAN) { return (m._bo) ? 1 : 0; }
        else if (m_t == ATOM_FLOAT4)  { return (LONGLONG)m._fl; }
        else if (m_t == ATOM_FLOAT8)  { return (LONGLONG)m._do; }
        else if (m_t == ATOM_BINARY)  
		{
            if (m._bin != NULL && m_q == 1) 
			{
                LONGLONG i = (LONGLONG)*(m._bin);
                return i;
            }
        }
        
		return 0;
	}
    
	int getIntAt(int pos) const 
	{
        int primtype;
		
		if ((m_t & ATOM_ARRAY) == 0)
        {
            return (pos == 0) ? this->getInt() : 0;
        }
        
		if (m._ptr == NULL || m_q <= pos)
        {
            return -1;
        }
        
		//int primtype = m_t & ~ATOM_ARRAY;
		primtype = m_t & ~ATOM_ARRAY;
        
		if (primtype == ATOM_INT4) 
		{ 
			return *(((int *)m._ptr) + pos); 
		}
        else if (primtype == ATOM_UINT4) 
		{
            return (int)(*(((UINT *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_INT2)  
		{
            return (int)(*(((short *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_INT1)  
		{
            return (int)(*(((char *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_UINT2) 
		{
            return (int)(*(((USHORT *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_UINT1) 
		{
            return (int)(*(((BYTE *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_INT8)  
		{
            return (int)(*(((LONGLONG *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_UINT8) 
		{
            return (int)(*(((ULONGLONG *)m._ptr) + pos));
        } 
		else if (primtype == ATOM_BOOLEAN) 
		{
            return (*(((bool *)m._ptr) + pos)) ? 1 : 0;
        } 
		else if (primtype == ATOM_FLOAT4)  
		{
            long l = (long)(*(((float *)m._ptr) + pos));
            return (int)l;
        } 
		else if (primtype == ATOM_FLOAT8)  
		{
            long l = (long)(*(((double *)m._ptr) + pos));
            return (int)l;
        } 
		else if (primtype == ATOM_BINARY)  
		{
            return -1;
        }
        
		return 0;
    }
    
	unsigned int getUInt() const 
	{
        if (m_t == ATOM_UINT4)        
		{ 
			return m._ui; 
		}
        else if (m_t == ATOM_INT4)    
		{ 
			return (UINT)m._in; 
		}
        else if (m_t == ATOM_UINT2)   
		{ 
			return (UINT)m._us; 
		}
        else if (m_t == ATOM_UINT1)   
		{ 
			return (UINT)m._uc; 
		}
        else if (m_t == ATOM_INT2)    
		{ 
			return (UINT)m._sh; 
		}
        else if (m_t == ATOM_INT1)    
		{ 
			return (UINT)m._ch; 
		}
        else if (m_t == ATOM_UINT8)   
		{ 
			return (UINT)m._ull; 
		}
        else if (m_t == ATOM_INT8)    
		{ 
			return (UINT)m._llo; 
		}
        else if (m_t == ATOM_BOOLEAN) 
		{ 
			return (m._bo) ? 1 : 0; 
		}
        else if (m_t == ATOM_FLOAT4)  
		{ 
			long l = (long)m._fl; 
			return (UINT)l; 
		}
        else if (m_t == ATOM_FLOAT8)  
		{ 
			long l = (long)m._do; 
			return (UINT)l; 
		}
        else if (m_t == ATOM_BINARY)  
		{
            if (m._bin != NULL && m_q == 1) 
			{
                UINT i = *(m._bin);
                return i;
            }
        }
        
		return 0;
	}
    
	ULONGLONG getUInt2() const 
	{
        if (m_t == ATOM_UINT8)			{ return m._ull; }
        else if (m_t == ATOM_UINT4)		{ return (ULONGLONG)m._ui; }
        else if (m_t == ATOM_INT4)		{ return (ULONGLONG)m._in; }
        else if (m_t == ATOM_UINT2)		{ return (ULONGLONG)m._us; }
        else if (m_t == ATOM_UINT1)		{ return (ULONGLONG)m._uc; }
        else if (m_t == ATOM_INT2)		{ return (ULONGLONG)m._sh; }
        else if (m_t == ATOM_INT1)		{ return (ULONGLONG)m._ch; }
        else if (m_t == ATOM_INT8)		{ return (ULONGLONG)m._llo; }
        else if (m_t == ATOM_BOOLEAN)	{ return (m._bo) ? 1 : 0; }
        else if (m_t == ATOM_FLOAT4)	{ return (ULONGLONG)m._fl; }
        else if (m_t == ATOM_FLOAT8)	{ return (ULONGLONG)m._do; }
        else if (m_t == ATOM_BINARY)  
		{
            if (m._bin != NULL && m_q == 1) 
			{
                ULONGLONG i = (ULONGLONG)*(m._bin);
                return i;
            }
        }
        return 0;
	}
    
	int setInt(int v);
    int setDouble(double v);

    int  set(const string& valstr, int format);
    void set(const b_value& value)  { *this = value; }
    void set(const BYTE * bin, size_t q)  {
            clean();
            m_t = ATOM_BINARY; m_q = q; m_sz = sizeof(BYTE);
            m._bin = (BYTE *)malloc(m_q * m_sz);
            memmove(m._bin, bin, q);
        }
    void set(b_valvect * any, int type) {  // type is STRUCT or VECTOR
            m._vec = any; m_t = type; m_sz = 0;
            m_q = any->size();
        }
    void get(b_value& retval) { retval = *this; }
    int  get(string& retval) const;

    virtual string toString() const;

    static const BCHAR * formatStr(int format);
    static const BCHAR * smlStr(int format);
    static int   strToFormat(const BCHAR * frmStr);
    static int   smlToFormat(const BCHAR * smlStr);
    static b_value zero;

    friend bool operator==(const b_value& lhs, const b_value& rhs);
    friend bool operator!=(const b_value& lhs, const b_value& rhs) {
            return (! (lhs == rhs));
        }
    friend bool operator<(const b_value& lhs, const b_value& rhs);

//
protected:
    void   copy(const b_value& rhs);
    size_t total() const;          // calculate bytes in list
};

inline bool operator==(const b_value& lhs, const b_value& rhs)
{
    if (lhs.m_t == rhs.m_t) {
        if (lhs.m_t == ATOM_ASCII) {
            return (*lhs.m._str == *rhs.m._str);
        } else if (lhs.m_t == ATOM_UINT4) {
            return (lhs.m._ui == rhs.m._ui);
        } else if (lhs.m_t == ATOM_INT4) {
            return (lhs.m._in == rhs.m._in);
        } else if (lhs.m_t == ATOM_BOOLEAN) {
            return (lhs.m._bo == rhs.m._bo);
        } else if (lhs.m_t == ATOM_UINT2) {
            return (lhs.m._us == rhs.m._us);
        } else if (lhs.m_t == ATOM_INT2) {
            return (lhs.m._sh == rhs.m._sh);
        } else if (lhs.m_t == ATOM_UINT1) {
            return (lhs.m._uc == rhs.m._uc);
        } else if (lhs.m_t == ATOM_INT1) {
            return (lhs.m._ch == rhs.m._ch);
        } else if (lhs.m_t == ATOM_FLOAT8) {
            return (lhs.m._do == rhs.m._do);
        } else if (lhs.m_t == ATOM_FLOAT4) {
            return (lhs.m._fl == rhs.m._fl);
        } else if (lhs.m_t == ATOM_INT8) {
            return (lhs.m._llo == rhs.m._llo);
        } else if (lhs.m_t == ATOM_UINT8) {
            return (lhs.m._ull == rhs.m._ull);
        } else if (lhs.m_t == ATOM_JIS || lhs.m_t == ATOM_UNICODE) {
            return (*lhs.m._str == *rhs.m._str);
        } else if (lhs.m_t == ATOM_LIST || lhs.m_t == ATOM_STRUCT ||
                   lhs.m_t == ATOM_VECTOR) {
            if (lhs.m._vec == NULL || rhs.m._vec == NULL)
                return (lhs.m._vec == rhs.m._vec);
            return (*lhs.m._vec == *rhs.m._vec);
        }
    } else if (lhs.m_t == ATOM_UINT4) {
        unsigned int uival = rhs.getUInt();
        return (lhs.m._ui == uival);
    } else if (lhs.m_t == ATOM_INT4) {
        int ival = rhs.getInt();
        return (lhs.m._in == ival);
    } else if (lhs.m_t == ATOM_INT8) {
        LONGLONG ival = rhs.getInt2();
        return (lhs.m._llo == ival);
    } else if (lhs.m_t == ATOM_UINT8) {
        ULONGLONG ival = rhs.getUInt2();
        return (lhs.m._ull == ival);
    }
    return false;
}

inline bool operator<(const b_value& lhs, const b_value& rhs)
{
    if (lhs.m_t == rhs.m_t) {
        if (lhs.m_t == ATOM_ASCII) {
            return (*lhs.m._str < *rhs.m._str);
        } else if (lhs.m_t == ATOM_UINT4) {
            return (lhs.m._ui < rhs.m._ui);
        } else if (lhs.m_t == ATOM_INT4) {
            return (lhs.m._in < rhs.m._in);
        } else if (lhs.m_t == ATOM_UINT2) {
            return (lhs.m._us < rhs.m._us);
        } else if (lhs.m_t == ATOM_INT2) {
            return (lhs.m._sh < rhs.m._sh);
        } else if (lhs.m_t == ATOM_UINT1) {
            return (lhs.m._uc < rhs.m._uc);
        } else if (lhs.m_t == ATOM_INT1) {
            return (lhs.m._ch < rhs.m._ch);
        } else if (lhs.m_t == ATOM_BOOLEAN) {
            return (lhs.m._bo < rhs.m._bo);
        } else if (lhs.m_t == ATOM_FLOAT8) {
            return (lhs.m._do < rhs.m._do);
        } else if (lhs.m_t == ATOM_FLOAT4) {
            return (lhs.m._fl < rhs.m._fl);
        } else if (lhs.m_t == ATOM_INT8) {
            return (lhs.m._llo < rhs.m._llo);
        } else if (lhs.m_t == ATOM_UINT8) {
            return (lhs.m._ull < rhs.m._ull);
        } else if (lhs.m_t == ATOM_JIS || lhs.m_t == ATOM_UNICODE) {
            return (*lhs.m._str < *rhs.m._str);
        } else if (lhs.m_t == ATOM_LIST || lhs.m_t == ATOM_STRUCT ||
                   lhs.m_t == ATOM_VECTOR) {
            if (lhs.m._vec == NULL || rhs.m._vec == NULL)
                return false;
            return (*lhs.m._vec < *rhs.m._vec);
        }
    } else if (lhs.m_t == ATOM_UINT4) {
        unsigned int uival = rhs.getUInt();
        return (lhs.m._ui < uival);
    } else if (lhs.m_t == ATOM_INT4) {
        int ival = rhs.getInt();
        return (lhs.m._in < ival);
    } else if (lhs.m_t == ATOM_INT8) {
        LONGLONG ival = rhs.getInt2();
        return (lhs.m._llo < ival);
    } else if (lhs.m_t == ATOM_UINT8) {
        ULONGLONG ival = rhs.getUInt2();
        return (lhs.m._ull < ival);
    }
    return false;
}

// Compare
struct value_less
{
     bool operator() (const b_value& x, const b_value& y) const {
             return (x < y);
         }
};

#endif  /* B_VALUE_H */
