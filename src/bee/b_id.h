// $Id: b_id.h,v 1.8 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_id.h
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

#ifndef B_ID_H
#define B_ID_H

#include "b_value.h"

class b_id : public b_value
{
public:
    b_id(): b_value() { m_t = ATOM_INT4; m._ui = 0; }
    b_id(int  id): b_value(id) {}
    b_id(UINT id): b_value(id) {}
    b_id(LONGLONG  id): b_value(id) {}
    b_id(ULONGLONG id): b_value(id) {}
    b_id(const string& s): b_value(s) {}
    b_id(const BCHAR * id): b_value(id) {}
    b_id(const b_id& rhs): b_value((b_value&)rhs) {}
    ~b_id() {}

    bool get(int& rid) {
            if (m_t == ATOM_ASCII) {
                rid = strtoul(m._str->c_str(), NULL, 0);
                return false;
            } else if (m_t == ATOM_INT4) {
                rid = m._in;
            } else if (m_t == ATOM_UINT4) {
                rid = (int)m._ui;
            } else {
                rid = 0;
                return false;
            }
            return true;
        }
    bool get(UINT& rid) {
            if (m_t == ATOM_ASCII) {
                rid = strtoul(m._str->c_str(), NULL, 0);
                return false;
            } else if (m_t == ATOM_INT4) {
                rid = (UINT)m._in;
            } else if (m_t == ATOM_UINT4) {
                rid = m._ui;
            } else {
                rid = 0;
                return false;
            }
            return true;
        }
    bool get(LONGLONG& rid) {
            if (m_t == ATOM_ASCII) {
#ifdef WIN32
                rid = _atoi64(m._str->c_str());
#else
                rid = strtoll(m._str->c_str(), NULL, 0);
#endif
                return false;
            } else if (m_t == ATOM_INT8) {
                rid = m._llo;
            } else if (m_t == ATOM_UINT8) {
                rid = (LONGLONG)m._ull;
            } else if (m_t == ATOM_INT4) {
                rid = (LONGLONG)m._in;
            } else if (m_t == ATOM_UINT4) {
                rid = (LONGLONG)m._ui;
            } else {
                rid = 0;
                return false;
            }
            return true;
        }
    bool get(ULONGLONG& rid) {
            if (m_t == ATOM_ASCII) {
#ifdef WIN32
                rid = _strtoui64(m._str->c_str(), NULL, 0);
#else
                rid = strtoull(m._str->c_str(), NULL, 0);
#endif
                return false;
            } else if (m_t == ATOM_INT8) {
                rid = (ULONGLONG)m._llo;
            } else if (m_t == ATOM_UINT8) {
                rid = m._ull;
            } else if (m_t == ATOM_INT4) {
                rid = (ULONGLONG)m._in;
            } else if (m_t == ATOM_UINT4) {
                rid = (ULONGLONG)m._ui;
            } else {
                rid = 0;
                return false;
            }
            return true;
        }

    bool get(string& rid) const {
            if (m_t == ATOM_ASCII) {
                rid = (m._str == NULL) ? _TX("") : *(m._str) ;
            } else {
                BCHAR buf[64];
                if (m_t == ATOM_INT4) {
                    _stprintf(buf, _TX("%d"), m._in);
                } else {
                    _stprintf(buf, _TX("%u"), m._ui);
                }
                rid = buf;
            }
            return true;
        }

    b_id& operator=(int id)       { init(id); return *this; }
    b_id& operator=(UINT id)      { init(id); return *this; }
    b_id& operator=(LONGLONG id)  { init(id); return *this; }
    b_id& operator=(ULONGLONG id) { init(id); return *this; }
    b_id& operator=(string& id)   { init(&id); return *this; }
    b_id& operator=(const BCHAR * id) {
            init(id);
            return *this;
        }
    b_id& operator=(const b_id& rhs) {
            if (this == &rhs)
                return *this;
            if (rhs.m_t == ATOM_ASCII)
                init(rhs.m._str);
            else if (rhs.m_t == ATOM_INT4)
                init(rhs.m._in);
            else if (rhs.m_t == ATOM_UINT4)
                init(rhs.m._ui);
            else if (rhs.m_t == ATOM_INT8)
                init(rhs.m._llo);
            else // if (rhs.m_t == ATOM_UINT8)
                init(rhs.m._ull);
            return *this;
        }

    friend bool operator==(const b_id& lhs, const b_id& rhs);
    friend bool operator!=(const b_id& lhs, const b_id& rhs) {
            return (! (lhs == rhs));
        }
    friend bool operator<(const b_id& lhs, const b_id& rhs);

//
protected:
    inline void init(const string * v) {
            clean();
            m_t = ATOM_ASCII;
            m_sz = sizeof(BCHAR);
            if (v == NULL) {
                m_q = 0; m._str = NULL;
            } else {
                m_q = v->size();
                if (m_q > 0)
                    m._str = new string(*v);
                else
                    m._str = NULL;
            }
        }
    inline void init(const BCHAR * v) {
            clean();
            m_t = ATOM_ASCII;
            m_sz = sizeof(BCHAR);
            if (v != NULL)
                m._str = new string(v);
            else
                m._str = NULL;
            m_q = m._str->size();
        }
    inline void init(int v) {
            clean();
            m_t = ATOM_INT4;
            m_q = 1;
            m_sz = sizeof(int);
            m._in = v;
        }
    inline void init(UINT v) {
            clean();
            m_t = ATOM_UINT4;
            m_q = 1;
            m_sz = sizeof(UINT);
            m._ui = v;
        }
    inline void init(LONGLONG v) {
            clean();
            m_t = ATOM_INT8;
            m_q = 1;
            m_sz = sizeof(LONGLONG);
            m._llo = v;
        }
    inline void init(ULONGLONG v) {
            clean();
            m_t = ATOM_UINT8;
            m_q = 1;
            m_sz = sizeof(ULONGLONG);
            m._ull = v;
        }
};

inline bool operator==(const b_id& lhs, const b_id& rhs)
{
    if (lhs.m_t == rhs.m_t) {
        if (lhs.m_t == ATOM_UINT4) {
            return (lhs.m._ui == rhs.m._ui);
        } else if (lhs.m_t == ATOM_INT4) {
            return (lhs.m._in == rhs.m._in);
        } else if (lhs.m_t == ATOM_INT8) {
            return (lhs.m._llo == rhs.m._llo);
        } else if (lhs.m_t == ATOM_UINT8) {
            return (lhs.m._ull == rhs.m._ull);
        } else {
            return (*lhs.m._str == *rhs.m._str);
        }
    }
    return false;
}

inline bool operator<(const b_id& lhs, const b_id& rhs)
{
    if (lhs.m_t == rhs.m_t) {
        if (lhs.m_t == ATOM_UINT4) {
            return (lhs.m._ui < rhs.m._ui);
        } else if (lhs.m_t == ATOM_INT4) {
            return (lhs.m._in < rhs.m._in);
        } else if (lhs.m_t == ATOM_INT8) {
            return (lhs.m._llo < rhs.m._llo);
        } else if (lhs.m_t == ATOM_UINT8) {
            return (lhs.m._ull < rhs.m._ull);
        } else {
            return (*lhs.m._str < *rhs.m._str);
        }
    }
    return false;
}

typedef vector<b_id>  b_ids;

struct id_less
{
     bool operator() (const b_id& x, const b_id& y) const { return (x < y); }
};

#endif  /* B_ID_H */
