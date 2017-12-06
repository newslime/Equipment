// $Id: BS2Stream.cpp,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Stream.cpp
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

#include "BS2Stream.h"
#include "BS2Message.h"
#include "BS2BlockHeader.h"
#include "BS2ItemHeader.h"
#include "BS2Array.h"
#include "BS2DeclAtoms.h"
#include "BS2ListItem.h"

//-----------------------------------------------------------------------------
// Stream Buffer
//-----------------------------------------------------------------------------
BS2StreamBuf::BS2StreamBuf(): m_state(0), m_endsave(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2StreamBuf::BS2StreamBuf()");

    m_msize = SECSBUF_ALLOCSIZE;
    char * buf = new char[m_msize];
    if (buf == NULL)
        m_msize = 0;
    setp(buf, buf + m_msize);
    setg(buf, buf, buf + m_msize);
}

//-----------------------------------------------------------------------------
BS2StreamBuf::BS2StreamBuf(char * buf, int n): m_state(0), m_endsave(NULL)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2StreamBuf::BS2StreamBuf()");

    m_msize = n;
    setp(buf, buf + n);
}

//-----------------------------------------------------------------------------
BS2StreamBuf::~BS2StreamBuf()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2StreamBuf::~BS2StreamBuf()");

    char * buf = eback();
    if (buf != NULL)
        delete [] buf;
}

//-----------------------------------------------------------------------------
int BS2StreamBuf::overflow(int c)
{
    int oldsize = (gptr() == NULL) ? 0 : pptr() - eback();
    int newsize = oldsize + SECSBUF_ALLOCSIZE;
    char * newbuf = new char[newsize];
    if (newbuf == NULL)
        return EOF;
    if (oldsize > 0)
        memcpy(newbuf, eback(), oldsize);
    delete [] eback();    // free old buffer
    if (oldsize == 0)
    {
        setp(newbuf, newbuf + newsize);
        setg(newbuf, newbuf, newbuf + newsize);
    }
    else
    {
#if defined(ACE_WIN32)
        setp(newbuf, newbuf + oldsize, newbuf + newsize);
#else
        setp(newbuf, newbuf + newsize);
        pbump(oldsize);
#endif
        setg(newbuf, newbuf + (gptr() - eback()), newbuf + newsize);
    }
    if (c == EOF)
        return c;
    sputc(c);
    return c;
}

//-----------------------------------------------------------------------------
streambuf * BS2StreamBuf::setbuf(char * s, int n)
{
    delete [] eback();    // free old buffer

    setg(s, s, s + n);
    setp(s, s + n);
    return (streambuf *)this;
}

//-----------------------------------------------------------------------------
BS2StreamBuf * BS2StreamBuf::xsetbuf(char * s, int n)
{
    setg(s, s, s + n);
    setp(s, s + n);
    return this;
}

//-----------------------------------------------------------------------------
void BS2StreamBuf::freeze(bool freezebit)
{
    if (freezebit && ((m_state & FROZEN) == 0))
    {
        m_state |= FROZEN;
        m_endsave = epptr();
#ifdef _MSC_VER
        setp(pbase(), pptr(), eback());
#else
        setg(pbase(), pptr(), eback());
#endif
    }
    else if (!freezebit && ((m_state & FROZEN) == 1))
    {
        m_state &= ~FROZEN;
#ifdef _MSC_VER
        setp(pbase(), pptr(), m_endsave);
#else
        setg(pbase(), pptr(), eback());
#endif
    }
}

void BS2StreamBuf::dump()
{
    if (eback() != NULL)
        mem_dump(eback(), epptr(), 0, NO_BOUNDARY);
}

//-----------------------------------------------------------------------------
// IStream
//-----------------------------------------------------------------------------
int BS2IStream::sgetItemHeader(BS2ItemHeader& itemhead)
{
    int result = BEE_ERROR;

    if (m_sb.gsize() > (m_sb.gcurptr() - m_sb.gtop()))
    {
        char * itemtop = (char *)&itemhead;
        char * datap = m_sb.gcurptr();
        *itemtop++ = *datap++;
        size_t n = itemhead.lenSize();
        if (n <= (sizeof(BS2ItemHeader) - 1))
        {
            for (size_t i = 0; i < n; i++)       // back next position
                *itemtop++ = *datap++;
            result = BEE_SUCCESS;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
int BS2IStream::nextItem(BS2ItemHeader& nextItemHeader)
{
    int				len;
	int				result;
	BS2ItemHeader	itemHeader;
    
	*this >> itemHeader; // abandon item header
    len = itemHeader.dataLength();
    
	//if( len < 0 )
	//	return -1;
	
	if (! itemHeader.isList())
        m_sb.gnext(len);

    result = sgetItemHeader(nextItemHeader);
    
	return result;
}

//-----------------------------------------------------------------------------
int BS2IStream::topItem(BS2ItemHeader& itemhead)
{
    int result;
    m_sb.greset();
    m_sb.gnext(sizeof(BS2BlockHeader));
    result = sgetItemHeader(itemhead);
    return result;
}

//-----------------------------------------------------------------------------
int BS2IStream::header(BS2BlockHeader& bheader)
{
    int result = BEE_SUCCESS;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        char * top = (char *)&bheader;
        memcpy(top, m_sb.gtop(), sizeof(BS2BlockHeader));
    }
    else
        result = BEE_ERROR;
    return result;
}

//-----------------------------------------------------------------------------
bool BS2IStream::isPrimary()
{
    bool result;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        BS2BlockHeader * bheader = (BS2BlockHeader *)m_sb.gtop();
        result = bheader->isPrimary();
    }
    else
        result = false;
    return result;
}

//-----------------------------------------------------------------------------
bool BS2IStream::isWait()
{
    bool result;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        BS2BlockHeader * bheader = (BS2BlockHeader *)m_sb.gtop();
        result = bheader->wait();
    }
    else
        result = false;
    return result;
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (bool& b)
{
    read((char *)&b, sizeof(bool));   // bool is a byte
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (char& c)
{
    read(&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (unsigned char& c)
{
    read((char *)&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (short& i)
{
    char data[sizeof(short)];
    read(data, sizeof(short));
    i = (data[0] << 8) + data[1];
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (unsigned short& i)
{
    char data[sizeof(unsigned short)];
    read(data, sizeof(unsigned short));
    i = (data[0] << 8) + data[1];
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (int& i)
{
    BYTE data[sizeof(int)];
    read((char *)data, sizeof(int));
    i = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (unsigned int& i)
{
    char data[sizeof(int)];
    read(data, sizeof(int));
    i = (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (LONGLONG& l)
{
    char * retptr = (char *)&l;
    char data[sizeof(LONGLONG)];
    read(data, sizeof(LONGLONG));
#if ENDIAN == LITTLE_ENDIAN
    *(retptr + 7) = data[0];
    *(retptr + 6) = data[1];
    *(retptr + 5) = data[2];
    *(retptr + 4) = data[3];
    *(retptr + 3) = data[4];
    *(retptr + 2) = data[5];
    *(retptr + 1) = data[6];
    *(retptr + 0) = data[7];
#else
    *(retptr + 0) = data[0];
    *(retptr + 1) = data[1];
    *(retptr + 2) = data[2];
    *(retptr + 3) = data[3];
    *(retptr + 4) = data[4];
    *(retptr + 5) = data[5];
    *(retptr + 6) = data[6];
    *(retptr + 7) = data[7];
#endif
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (ULONGLONG& l)
{
    char * retptr = (char *)&l;
    char data[sizeof(ULONGLONG)];
    read(data, sizeof(ULONGLONG));
#if ENDIAN == LITTLE_ENDIAN
    *(retptr + 7) = data[0];
    *(retptr + 6) = data[1];
    *(retptr + 5) = data[2];
    *(retptr + 4) = data[3];
    *(retptr + 3) = data[4];
    *(retptr + 2) = data[5];
    *(retptr + 1) = data[6];
    *(retptr + 0) = data[7];
#else
    *(retptr + 0) = data[0];
    *(retptr + 1) = data[1];
    *(retptr + 2) = data[2];
    *(retptr + 3) = data[3];
    *(retptr + 4) = data[4];
    *(retptr + 5) = data[5];
    *(retptr + 6) = data[6];
    *(retptr + 7) = data[7];
#endif
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (float& f)
{
    char * retptr = (char *)&f;
    char data[sizeof(float)];
    read(data, sizeof(float));
#if ENDIAN == LITTLE_ENDIAN
    *(retptr + 3) = data[0];
    *(retptr + 2) = data[1];
    *(retptr + 1) = data[2];
    *(retptr + 0) = data[3];
#else
    *(retptr + 0) = data[0];
    *(retptr + 1) = data[1];
    *(retptr + 2) = data[2];
    *(retptr + 3) = data[3];
#endif
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (double& d)
{
    char * retptr = (char *)&d;
    char data[sizeof(double)];
    read(data, sizeof(double));
#if ENDIAN == LITTLE_ENDIAN
    *(retptr + 7) = data[0];
    *(retptr + 6) = data[1];
    *(retptr + 5) = data[2];
    *(retptr + 4) = data[3];
    *(retptr + 3) = data[4];
    *(retptr + 2) = data[5];
    *(retptr + 1) = data[6];
    *(retptr + 0) = data[7];
#else
    *(retptr + 0) = data[0];
    *(retptr + 1) = data[1];
    *(retptr + 2) = data[2];
    *(retptr + 3) = data[3];
    *(retptr + 4) = data[4];
    *(retptr + 5) = data[5];
    *(retptr + 6) = data[6];
    *(retptr + 7) = data[7];
#endif
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (BS2ItemHeader& itemhead)
{
    char * itemtop = (char *)&itemhead;
    int l = read(itemtop, 1);
    if (l == 1)
    {
        int n = itemhead.lenSize();
        l = read(itemtop + 1, n);
        if (l != n)
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("%s: unexpected to read data size. \n"),
                       ACE_TEXT("BS2IStream::operator>>BS2ItemHeader&")));
    }
    return (*this);
}

//-----------------------------------------------------------------------------
BS2IStream& BS2IStream::operator>> (BS2Message& head)
{
    BS2BlockHeader block;
    int l = read((char*)&block, sizeof(BS2BlockHeader));
    if (l == sizeof(BS2BlockHeader))
    {
        head.set(&block);
    }
    return (*this);
}

//-----------------------------------------------------------------------------
// input secs2 stream function
//-----------------------------------------------------------------------------
int BS2IStream::makeAtoms(BS2Atoms& atoms)
{
    int				result;
	int				elementNum;
    BS2ItemHeader	itemHeader;
	BS2ItemHeader	dummyItemHeader;
    BS2Atom*		atom;
	BS2List*		listAtom;

    if (size() > (int)sizeof(BS2BlockHeader))
    {
        result = topItem(itemHeader);
        
		// Only header message
		if (result < 0)
        {
            return 0;
        }
        
		do
        {
            if (itemHeader.isList())
            {
                elementNum = itemHeader.dataLength();
                
				if( elementNum < 0 ) //by stone
					return -1;
				
				//BS2ItemHeader dummyItemHeader;
                *this >> dummyItemHeader; //abandon item header
                listAtom = new BS2List;
                atoms.push_back((BS2Atom *)listAtom);
                listElements(listAtom, elementNum);
                result = sgetItemHeader(itemHeader);
            }
            else
            {
                const BS2ItemHeader* itemptr = (const BS2ItemHeader *)curptr();
                atom = BS2Atom::factory(*itemptr);
                
				if (atom == NULL)
                    return -1;
                
				atoms.push_back(atom);
                result = nextItem(itemHeader);
            }
        } while (result >= 0);

    }
    
	return 0;
}

//-----------------------------------------------------------------------------
// input list element
//-----------------------------------------------------------------------------
int BS2IStream::listElements(BS2List * listParent, int elementNum)
{
    int				i, result;
	int				nestElementNum;
    BS2ItemHeader	itemHeader;
	BS2ItemHeader	dummyItemHeader;
    BS2Atom*		atom;
	BS2List*		listChild;

    result = sgetItemHeader(itemHeader);
    
	for (i = 0; i < elementNum; i++)
    {
        if (itemHeader.isList())
        {
            nestElementNum = itemHeader.dataLength();
           
			if( nestElementNum < 0 ) //by stone
			{
				result = -1;
				break;
			}
           
			 //BS2ItemHeader dummyItemHeader;
			*this >> dummyItemHeader;          // abandon item header
            listChild = new BS2List;
            listParent->getList().push_back((BS2Atom *)listChild);
            listElements(listChild, nestElementNum);
            result = sgetItemHeader(itemHeader);
            if (result < 0)
                break;
        }
        else
        {
            const BS2ItemHeader* itemptr = (const BS2ItemHeader *)curptr();
            atom = BS2Atom::factory(*itemptr);
            listParent->getList().push_back(atom);
            result = nextItem(itemHeader);
            if (result < 0)
                break;
        }
    }
    
	return result;
}


//-----------------------------------------------------------------------------
// OStream
//-----------------------------------------------------------------------------
/*
bool BS2OStream::isPrimary()
{
    bool result;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        BS2BlockHeader * bheader = (BS2BlockHeader *)m_sb.gtop();
        result = bheader->isPrimary();
    }
    else
        result = false;
    return result;
}*/

bool BS2OStream::isPrimary()
{
    bool result;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        BS2BlockHeader * bheader = (BS2BlockHeader *)m_sb.gtop();
		  if(bheader->m_hiMsgId==9&&bheader->m_loMsgId==7)
		  {
				 result = false;
		  }
		  else
		  {
				result = bheader->isPrimary();
		  }
	}
    else
        result = false;
   
	return result;
}

//-----------------------------------------------------------------------------
bool BS2OStream::isWait()
{
    bool result;
    if (m_sb.gsize() >= (int)sizeof(BS2BlockHeader))
    {
        BS2BlockHeader * bheader = (BS2BlockHeader *)m_sb.gtop();
        result = bheader->wait();
    }
    else
        result = false;
    return result;
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (bool& b)
{
    write((char *)&b, sizeof(bool));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (char& c)
{
    write(&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const char& c)
{
    write(&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (unsigned char& c)
{
    write((char *)&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const unsigned char& c)
{
    write((char *)&c, 1);
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (short& i)
{
    char data[sizeof(short)];

    data[0] = (i >> 8) & 0xFF;
    data[1] = i & 0xFF;

    write(data, sizeof(short));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const short& i)
{
    char data[sizeof(short)];

    data[0] = (i >> 8) & 0xFF;
    data[1] = i & 0xFF;

    write(data, sizeof(short));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (unsigned short& i)
{
    char data[sizeof(unsigned short)];

    data[0] = (i >> 8) & 0xFF;
    data[1] = i & 0xFF;

    write(data, sizeof(unsigned short));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const unsigned short& i)
{
    char data[sizeof(unsigned short)];

    data[0] = (i >> 8) & 0xFF;
    data[1] = i & 0xFF;

    write(data, sizeof(unsigned short));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (int& i)
{
    BYTE data[sizeof(int)];

    data[0] = (i >> 24) & 0xFF;
    data[1] = (i >> 16) & 0xFF;
    data[2] = (i >> 8) & 0xFF;
    data[3] = i & 0xFF;

    write((char *)data, sizeof(int));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const int& i)
{
    char data[sizeof(int)];

    data[0] = (i >> 24) & 0xFF;
    data[1] = (i >> 16) & 0xFF;
    data[2] = (i >> 8) & 0xFF;
    data[3] = i & 0xFF;

    write((char *)data, sizeof(int));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (unsigned int& i)
{
    char data[sizeof(int)];

    data[0] = (i >> 24) & 0xFF;
    data[1] = (i >> 16) & 0xFF;
    data[2] = (i >> 8) & 0xFF;
    data[3] = i & 0xFF;
    write(data, sizeof(int));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const unsigned int& i)
{
    char data[sizeof(int)];

    data[0] = (i >> 24) & 0xFF;
    data[1] = (i >> 16) & 0xFF;
    data[2] = (i >> 8) & 0xFF;
    data[3] = i & 0xFF;
    write(data, sizeof(int));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (LONGLONG& l)
{
    char * sptr = (char *)&l;
    char data[sizeof(LONGLONG)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(LONGLONG));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const LONGLONG& l)
{
    char * sptr = (char *)&l;
    char data[sizeof(LONGLONG)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(LONGLONG));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (ULONGLONG& l)
{
    char * sptr = (char *)&l;
    char data[sizeof(ULONGLONG)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(ULONGLONG));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const ULONGLONG& l)
{
    char * sptr = (char *)&l;
    char data[sizeof(ULONGLONG)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(ULONGLONG));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (float& f)
{
    char * sptr = (char *)&f;
    char data[sizeof(float)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 3);
    data[1] = *(sptr + 2);
    data[2] = *(sptr + 1);
    data[3] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
#endif
    write(data, sizeof(float));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const float& f)
{
    char * sptr = (char *)&f;
    char data[sizeof(float)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 3);
    data[1] = *(sptr + 2);
    data[2] = *(sptr + 1);
    data[3] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
#endif
    write(data, sizeof(float));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (double& d)
{
    char * sptr = (char *)&d;
    char data[sizeof(double)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(double));
    return (*this);
}

BS2OStream& BS2OStream::operator<< (const double& d)
{
    char * sptr = (char *)&d;
    char data[sizeof(double)];
#if ENDIAN == LITTLE_ENDIAN
    data[0] = *(sptr + 7);
    data[1] = *(sptr + 6);
    data[2] = *(sptr + 5);
    data[3] = *(sptr + 4);
    data[4] = *(sptr + 3);
    data[5] = *(sptr + 2);
    data[6] = *(sptr + 1);
    data[7] = *(sptr + 0);
#else
    data[0] = *(sptr + 0);
    data[1] = *(sptr + 1);
    data[2] = *(sptr + 2);
    data[3] = *(sptr + 3);
    data[4] = *(sptr + 4);
    data[5] = *(sptr + 5);
    data[6] = *(sptr + 6);
    data[7] = *(sptr + 7);
#endif
    write(data, sizeof(double));
    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const BS2Message& head)
{
    BS2BlockHeader block;
    head.get(&block);
    write((char *)&block, sizeof(BS2BlockHeader));

    return (*this);
}

//-----------------------------------------------------------------------------
BS2OStream& BS2OStream::operator<< (const BS2ItemHeader& itemhead)
{
    int n = itemhead.lenSize() + 1;
    write((char *)&itemhead, n);

    return (*this);
}

//-----------------------------------------------------------------------------
int BS2OStream::blockHeader(BS2BlockHeader& bheader)
{
    int result = BEE_SUCCESS;
    if (m_sb.pcount() >= (int)sizeof(BS2BlockHeader))
    {
        char * top = (char *)&bheader;
        memcpy(top, m_sb.ptop(), sizeof(BS2BlockHeader));
    }
    else
        result = BEE_ERROR;
    return result;
}

//-----------------------------------------------------------------------------
// make istream
//-----------------------------------------------------------------------------
BS2IStream * BS2OStream::make() const
{
    char * outdata = m_sb.ptop();
    int  size = pcount();
    BS2IStream * istmbuf = new BS2IStream;
    char * nbuf = new char[size];
    if (nbuf == NULL)
    {
        return NULL;
    }

    memcpy(nbuf, outdata, size);
    istmbuf->setdata(nbuf, size);
    return istmbuf;
}


//-----------------------------------------------------------------------------
void BS2OStream::print(void *)
{
    char * top = m_sb.str();
    int  size = m_sb.pcount();
    mem_dump(top, top + size, 0, NO_BOUNDARY);
    m_sb.freeze(false);
}

//-----------------------------------------------------------------------------
// output element
//-----------------------------------------------------------------------------
bool BS2OStream::set(BS2Message * msg)
{
    if (msg == NULL)
        throw invalid_argument(_TX("BS2OStream::set()"));

    *this << *msg;

    if (msg->itemCount() >= 0)
    {
        // output item data
        BS2ItemList::iterator iter;
        for (iter = msg->items().begin(); iter != msg->items().end();
             ++iter)
        {
            BS2Item * item = *iter;
            BS2Atom * atom = item->atom();
            if (item->isList())
            {
                BS2Items * items = &((BS2ListItem *)item)->items();
                BS2ItemHeader itemHeader(ATOM_LIST, items->size());
                *this << itemHeader;
                setList(items);
            }
            else if (atom != NULL)
            {
                atom->get(*this);
            }
            else
            {
                BS2ItemHeader itemHeader(ATOM_UINT2, 1, 0);
                *this << itemHeader;
            }
        }
		return true;
    }
	else
	{
		return false;
	}
}

//-----------------------------------------------------------------------------
// output element
//-----------------------------------------------------------------------------
bool BS2OStream::setList(BS2Items * items)
{
    // output item data
    BS2Items::iterator iter;
    for (iter = items->begin(); iter != items->end(); ++iter)
    {
        BS2Item * item = *iter;
        BS2Atom * atom = item->atom();
        if (item->isListItem())
        {
            BS2Items * nestItems = &((BS2ListItem *)item)->items();
            BS2ItemHeader itemHeader(ATOM_LIST, nestItems->size());
            *this << itemHeader;
            setList(nestItems);
        }
        else
        {
            atom->get(*this);
        }
    }
    return true;
}


//-----------------------------------------------------------------------------
void BS2OStream::dump()
{
    char * top = m_sb.str();
    int  size = m_sb.pcount();
    mem_dump(top, top + size, 0, NO_BOUNDARY);
    m_sb.freeze(false);
}


