// $Id: BS2Array.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Array.h
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

#ifndef BS2ARRAY_H
#define BS2ARRAY_H

#include "BS2Atom.h"

class BS2OStream;
class BS2IStream;
class BS2Int1;
class BS2Int2;
class BS2Int4;
class BS2Int8;
class BS2UInt1;
class BS2UInt2;
class BS2UInt4;
class BS2UInt8;
class BS2Float4;
class BS2Float8;

/* ------------------------------------------------------------------------- */
template <class T>
class BS2Array : public BS2Atom
{
public:
    BS2Array();
    BS2Array(T datum);
    BS2Array(BYTE * data, size_t size = sizeof(T));
    virtual ~BS2Array();
    virtual void dump() const;
    virtual void print(BS2InterpBase * interp) const;
    virtual void get(BS2OStream& buf) const;
    virtual void set(BS2IStream& buf);
    virtual void getStreamData(BYTE * buf) const;
    virtual void setStreamData(BYTE * buf);
    virtual BS2Atom * factory(BYTE * data, size_t len) const;
    virtual BS2Atom * replicate() const;

    const BS2Array<T>& operator=(const BS2Array<T>& rhs);
    int count() const { return m_q; }
    T * value() const { return (T *)m._ptr; }
    void add(BS2Array<T> * src);
    void add(T * data, int size = sizeof(T));
    void add(T datum);
    string arrayName() const;

protected:

};

template<class T>
BS2IStream& operator>>(BS2IStream& is, BS2Array<T>& atomArray);
template<class T>
BS2OStream& operator<<(BS2OStream& os, const BS2Array<T>& atomArray);

/* ------------------------------------------------------------------------- */
typedef BS2Array<bool>      BS2BoolArray;
typedef BS2Array<BCHAR>     BS2Int1Array;
typedef BS2Array<short>     BS2Int2Array;
typedef BS2Array<int>       BS2Int4Array;
typedef BS2Array<LONGLONG>  BS2Int8Array;
typedef BS2Array<BYTE>      BS2UInt1Array;
typedef BS2Array<USHORT>    BS2UInt2Array;
typedef BS2Array<UINT>      BS2UInt4Array;
typedef BS2Array<ULONGLONG> BS2UInt8Array;
typedef BS2Array<float>     BS2Float4Array;
typedef BS2Array<double>    BS2Float8Array;

/* ------------------------------------------------------------------------- */
//
//#include "BS2Array.i"
//

inline int toFormat(bool   v) { return ATOM_BOOLEAN; }
inline int toFormat(BCHAR   v) { return ATOM_INT1; }
inline int toFormat(short  v) { return ATOM_INT2; }
inline int toFormat(int    v) { return ATOM_INT4; }
inline int toFormat(BYTE   v) { return ATOM_UINT1; }
inline int toFormat(USHORT v) { return ATOM_UINT2; }
inline int toFormat(UINT   v) { return ATOM_UINT4; }
inline int toFormat(float v)  { return ATOM_FLOAT4; }
inline int toFormat(double v) { return ATOM_FLOAT8; }
inline int toFormat(LONGLONG  v) { return ATOM_INT8; }
inline int toFormat(ULONGLONG v) { return ATOM_UINT8; }

// $Id: BS2Array.i,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Array.i
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

#include "BS2DeclAtoms.h"
#include "BS2ItemHeader.h"
#include "BS2Array.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"
#include "BS2value.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
template <class T>
BS2Array<T>::BS2Array()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Array<T>::BS2Array");

    init(toFormat((T)0), 0, sizeof(T));
    m._ptr = NULL;
}

//-----------------------------------------------------------------------------
template <class T>
BS2Array<T>::BS2Array(T datum)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Array<T>::BS2Array");

    init(toFormat((T)0), 0, sizeof(T));
    T * values = (T *)malloc(sizeof(T));
    if (values != NULL)
    {
        m_q = 1;
        BYTE * buf = (BYTE *)&datum;
        BYTE * castPtr = (BYTE *)values;
#if ENDIAN == LITTLE_ENDIAN
        if (sizeof(T) == 1)
        {
            *values = datum;
        }
        else if (sizeof(T) == 2)
        {
            *(castPtr + 0) = *(buf + 1);
            *(castPtr + 1) = *(buf + 0);
        }
        else if (sizeof(T) == 4)
        {
            *(castPtr + 0) = *(buf + 3);
            *(castPtr + 1) = *(buf + 2);
            *(castPtr + 2) = *(buf + 1);
            *(castPtr + 3) = *(buf + 0);
        }
        else if (sizeof(T) == 8)
        {
            *(castPtr + 0) = *(buf + 7);
            *(castPtr + 1) = *(buf + 6);
            *(castPtr + 2) = *(buf + 5);
            *(castPtr + 3) = *(buf + 4);
            *(castPtr + 4) = *(buf + 3);
            *(castPtr + 5) = *(buf + 2);
            *(castPtr + 6) = *(buf + 1);
            *(castPtr + 7) = *(buf + 0);
        }
        else
        {
            BS2Panic("can't support size");
        }
#else
        for (size_t i = 0; i < sizeof(T); i++)
        {
            *(castPtr + i) = *(buf + i);
        }
#endif
    }
    m._ptr = values;
}

//-----------------------------------------------------------------------------
template <class T>
BS2Array<T>::BS2Array(BYTE * data, size_t size)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Array<T>::BS2Array");

    init(toFormat((T)0), size / sizeof(T), sizeof(T));
    T * values = (T *)malloc(size);
    if (values != NULL)
    {
#if ENDIAN == LITTLE_ENDIAN
        T * src = (T *)data;
        if (sizeof(T) == 1)
        {
            for (int i = 0; i < m_q; i++)
            {
                *(values + i) = *(src + i);
            }
        }
        else if (sizeof(T) == 2)
        {
            for (int i = 0; i < m_q; i++)
            {
                BYTE * buf = (BYTE *)&src[i];
                BYTE * castPtr = (BYTE *)&values[i];
                *(castPtr + 0) = *(buf + 1);
                *(castPtr + 1) = *(buf + 0);
            }
        }
        else if (sizeof(T) == 4)
        {
            for (int i = 0; i < m_q; i++)
            {
                BYTE * buf = (BYTE *)&src[i];
                BYTE * castPtr = (BYTE *)&values[i];
                *(castPtr + 0) = *(buf + 3);
                *(castPtr + 1) = *(buf + 2);
                *(castPtr + 2) = *(buf + 1);
                *(castPtr + 3) = *(buf + 0);
            }
        }
        else if (sizeof(T) == 8)
        {
            for (int i = 0; i < m_q; i++)
            {
                BYTE * buf = (BYTE *)&src[i];
                BYTE * castPtr = (BYTE *)&values[i];
                *(castPtr + 0) = *(buf + 7);
                *(castPtr + 1) = *(buf + 6);
                *(castPtr + 2) = *(buf + 5);
                *(castPtr + 3) = *(buf + 4);
                *(castPtr + 4) = *(buf + 3);
                *(castPtr + 5) = *(buf + 2);
                *(castPtr + 6) = *(buf + 1);
                *(castPtr + 7) = *(buf + 0);
            }
        }
        else
        {
            BS2Panic("can't support size");
        }
#else
        BYTE * castPtr = (BYTE *)values;
        for (size_t i = 0; i < size; i++)
        {
            *(castPtr + i) = *(data + i);
        }
#endif
    }
    m._ptr = values;
}

//-----------------------------------------------------------------------------
template <class T>
BS2Array<T>::~BS2Array()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Array<T>::BS2Array");
    if (m._ptr != NULL)
        free(m._ptr);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
template <class T>
const BS2Array<T>& BS2Array<T>::operator=(const BS2Array<T>& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::operator=");

    if (this == &rhs)
        return *this;
    if (m._ptr != NULL)
        free(m._ptr);

    m_q = rhs.m_q;
    T * data = (T *)rhs.m._ptr;
    int len = rhs.size();
    T * values = (T *)malloc(len);
    if (values != NULL)
    {
        memcpy(values, data, len);
    }
    m._ptr = values;
    return *this;
}

//-----------------------------------------------------------------------------
// Inquire type
//-----------------------------------------------------------------------------
template <class T>
string BS2Array<T>::arrayName() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::formatName");
    char buf[32];
    sprintf(buf, "Array%s", formatName());
    return buf;
}

//-----------------------------------------------------------------------------
// set SECS-II data
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::set(BS2IStream& buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::set");

    BS2ItemHeader item;
    buf >> item;
    int len = item.dataLength();
    if (m._ptr != NULL)
        free(m._ptr);

    m_q = len / size();
    T * values = (T *)malloc(len);
    if (values != NULL)
    {
        for (int i = 0; i < m_q; i++)
        {
            buf >> *(values + i);
        }
    }
    m._ptr = values;
}

//-----------------------------------------------------------------------------
// set value from stream buf
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::setStreamData(BYTE * tricky)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::setStreamData");
#ifdef JYUGEM_DECLARATION
    BS2values * valvect = (BS2values *)tricky;

    m_q = valvect.size();
    if (m._ptr != NULL)
        free(m._ptr);

    int len = m_q * size();
    T * values = (T *)malloc(len);
    if (values != NULL)
    {
        BS2value v;
        BS2values::iterator iter = valvect->begin();
        int i = 0;
        for ( ; iter != valvect->end(); ++iter, i++)
        {
            v = *iter;
            *(values + i) = v;
        }
    }
    m._ptr = values;
#else
    ACE_UNUSED_ARG(tricky);
#endif
}

//-----------------------------------------------------------------------------
// get SECS-II data
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::get(BS2OStream& buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::get");
    // buf size is small !

    BS2ItemHeader itemHeader(format(), size());
    buf << itemHeader;
    T * values = (T *)m._ptr;
    for (int i = 0; i < m_q; i++)
    {
        buf << *(values + i);
    }
}

//-----------------------------------------------------------------------------
// get value in stream buf
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::getStreamData(BYTE * tricky) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::getStreamData");
#ifdef JYUGEM_DECLARATION
    BS2values * valvect = (BS2values *)tricky;
    BS2value v;
    T * values = m._ptr;
    for (int i = 0; i < m_q; i++)
    {
        v = *(values + i);
        valvect->push_back(v);
    }
#else
    ACE_UNUSED_ARG(tricky);
#endif
}

//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
template <class T>
BS2Atom * BS2Array<T>::factory(BYTE * data, size_t len) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::factory");
    BS2Array<T> * clone = new BS2Array<T>(data, len);
    return (BS2Atom *)clone;
}

//-----------------------------------------------------------------------------
template <class T>
BS2Atom * BS2Array<T>::replicate() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::replicate");

    BS2Array<T> * replica = new BS2Array<T>;
    *replica = *this;
    return (BS2Atom *)replica;
}

//-----------------------------------------------------------------------------
// Add
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::add(BS2Array<T> * array)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::add");

    int newSize = (m_q + array->count()) * size();
    T * newBins = (T *)realloc(m._ptr, newSize);
    if (newBins == NULL)
    {
        BS2Panic("lack system memory");
    }
    memcpy(newBins + size(), array->value(), array->size());
    m_q += array->count();
    m._ptr = newBins;
}

//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::add(T * data, int len)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::add");

    int newSize = (m_q + len) * m_sz;
    T * newBins = (T *)realloc(m._ptr, newSize);
    if (newBins == NULL)
    {
        BS2Panic("lack system memory");
    }
    memcpy(newBins + this->size(), data, len * m_sz);
    m_q += len;
    m._ptr = newBins;
}

//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::add(T datum)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array<T>::add");

    int newSize = (m_q + 1) * m_sz;
    T * newBins = (T *)realloc(m._ptr, newSize);
    if (newBins == NULL)
    {
        BS2Panic("lack system memory");
    }
    // *(newBins + count()) = datum;
    memcpy(newBins + count(), &datum, sizeof(T));
    m_q += 1;
    m._ptr = newBins;
}

//-----------------------------------------------------------------------------
// io stream
//-----------------------------------------------------------------------------
template <class T>
BS2IStream& operator>>(BS2IStream& is, BS2Array<T>& atomArray)
{
    BS2ItemHeader itemHeader;
    is >> itemHeader;
    int len = itemHeader.dataLength();
    if (atomArray.m._ptr != NULL)
        free(atomArray.m._ptr);

    atomArray.m_val.m_q = len / atomArray.m_sz;
    T * values = (T *)malloc(len);
    if (values != NULL)
    {
        for (int i = 0; i < atomArray.m_q; i++)
        {
            is >> *(values + i);
        }
    }
    atomArray.m._ptr = values;
    return is;
}

//-----------------------------------------------------------------------------
template <class T>
BS2OStream& operator<<(BS2OStream& os, const BS2Array<T>& atomArray)
{
    BS2ItemHeader itemHeader;
    int length = atomArray.m_q * atomArray.m_sz;
    itemHeader.set(atomArray.format() & ~ATOM_ARRAY, length);
    os << itemHeader;
    for (int i = 0; i < atomArray.m_q; i++)
    {
        os << *(atomArray.m._ptr + i);
    }
    return os;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array::print");

    ACE_UNUSED_ARG(interp);
    return ;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
template <class T>
void BS2Array<T>::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Array::dump");

}

//
// *** End of File ***

#endif /* BS2ARRAY_H */
