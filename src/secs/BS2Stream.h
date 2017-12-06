// $Id: BS2Stream.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Stream.h
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

#ifndef BS2STREAM_H
#define BS2STREAM_H

#include "BS2Atom.h"

//-----------------------------------------------------------------------------
class BS2Atom;
class BS2Item;
class BS2List;
class BS2Message;
class BS2MsgHeader;
struct BS2ItemHeader;
struct BS2BlockHeader;

#ifndef BS2ITEMS_DECLARE
#define BS2ITEMS_DECLARE
typedef list<BS2Item *, allocator<BS2Item *> > BS2Items;
#endif

const int SECSBUF_ALLOCSIZE = 512;

//-----------------------------------------------------------------------------
// SECS Stream Buffer
//-----------------------------------------------------------------------------
class BEE_Export BS2StreamBuf: public streambuf {
public:
    enum { FROZEN = 1 };

    BS2StreamBuf();
    BS2StreamBuf(char * buf, int n);
    virtual ~BS2StreamBuf();
    void freeze(bool = true);
    char * str()           { freeze(); return (gptr()); }
    int  pcount() const    { return (pptr() == NULL ? 0 : pptr() - pbase()); }
    char * pcurptr() const { return (pptr()); }
    char * gcurptr() const { return (gptr()); }
    void pnext(int n)      { pbump(n); }
    void gnext(int n)      { gbump(n); }
    char * gtop() const    { return (eback()); }
    char * ptop() const    { return (pbase()); }
    int  gsize() const     { return (egptr() - eback()); }
    void greset()          { setg(eback(), eback(), egptr()); }
    void preset()          { setp(pbase(), epptr()); }
    void setdata(char * s, int n)  { setbuf(s, n); }
    void xsetdata(char * s, int n) { xsetbuf(s, n); }  // not delete old buffer
    int  read(char * s, int n)     { return xsgetn(s, n); }

    void dump();
protected:
    virtual int overflow(int c = EOF);
    virtual int underflow() { return EOF; }
    virtual streambuf * setbuf(char * s, int n);
    BS2StreamBuf * xsetbuf(char * s, int n);

private:
    int m_msize;
    int m_state;
    char * m_endsave;
};

//-----------------------------------------------------------------------------
// SECS IStream
//-----------------------------------------------------------------------------
class BEE_Export BS2IStream: public istream {
public:
    BS2IStream(): istream(&m_sb) {}
    BS2IStream(const char * buf, int n): istream(&m_sb), m_sb((char *)buf, n) {}
    BS2IStream(char * buf, int n): istream(&m_sb), m_sb(buf, n) {}
    virtual ~BS2IStream() { }

    int read(char * s, int n) { return m_sb.read(s, n); }
    BS2StreamBuf * rdbuf() const { return ((BS2StreamBuf *)&m_sb); }
    char * str()  { return (m_sb.str()); }
    char * gtop() { return (m_sb.gtop()); }
    void reset()  {  m_sb.greset(); }
    void setdata(char * datap, int size) { m_sb.setdata(datap, size); }
    char * curptr() const { return (m_sb.gcurptr()); }
    int  size() const { return (m_sb.gsize()); }
    BS2IStream& operator>> (bool& b);
    BS2IStream& operator>> (char& c);
    BS2IStream& operator>> (unsigned char& c);
    BS2IStream& operator>> (short& i);
    BS2IStream& operator>> (unsigned short& i);
    BS2IStream& operator>> (int& i);
    BS2IStream& operator>> (unsigned int& i);
    BS2IStream& operator>> (LONGLONG& l);
    BS2IStream& operator>> (ULONGLONG& l);
    BS2IStream& operator>> (float& f);
    BS2IStream& operator>> (double& d);
    BS2IStream& operator>> (BS2Message& msg);
    BS2IStream& operator>> (BS2ItemHeader& itemhead);
    int  sgetItemHeader(BS2ItemHeader& itemhead);
    int  header(BS2BlockHeader& bheader);
    int  topItem(BS2ItemHeader& itemhead);
    int  nextItem(BS2ItemHeader& nextItemHeader);
    bool isDataOver();
    bool isPrimary();
    bool isWait();
    int  makeAtoms(BS2Atoms& retptr);
    int  listElements(BS2List * listParent, int elementNum);

    void dump() { m_sb.dump(); }
private:
    BS2StreamBuf m_sb;

};

inline bool BS2IStream::isDataOver()
{
    return (peek() == EOF);
}

//-----------------------------------------------------------------------------
// SECS OStream
//-----------------------------------------------------------------------------
class BEE_Export BS2OStream: public ostream {
public:
    BS2OStream(): ostream(&m_sb), m_sb() {}
    virtual ~BS2OStream() { }

    BS2StreamBuf *rdbuf() const { return ((BS2StreamBuf *)&m_sb); }
    void reset()              { m_sb.preset(); }
    void freeze(bool bit)     { m_sb.freeze(bit); }
    char * str()              { return (m_sb.str()); }
    char * ptop()             { return (m_sb.ptop()); }
    streamsize pcount() const { return (m_sb.pcount()); }
    void setbuf(char * bufp, int size) { m_sb.xsetdata(bufp, size); }
    BS2OStream& operator<< (bool& b);
    BS2OStream& operator<< (char& c);
    BS2OStream& operator<< (const char& c);
    BS2OStream& operator<< (unsigned char& c);
    BS2OStream& operator<< (const unsigned char& c);
    BS2OStream& operator<< (short& i);
    BS2OStream& operator<< (const short& i);
    BS2OStream& operator<< (unsigned short& i);
    BS2OStream& operator<< (const unsigned short& i);
    BS2OStream& operator<< (int& i);
    BS2OStream& operator<< (const int& i);
    BS2OStream& operator<< (unsigned int& i);
    BS2OStream& operator<< (const unsigned int& i);
    BS2OStream& operator<< (LONGLONG& l);
    BS2OStream& operator<< (const LONGLONG& l);
    BS2OStream& operator<< (ULONGLONG& l);
    BS2OStream& operator<< (const ULONGLONG& l);
    BS2OStream& operator<< (float& f);
    BS2OStream& operator<< (const float& f);
    BS2OStream& operator<< (double& d);
    BS2OStream& operator<< (const double& d);
    BS2OStream& operator<< (const BS2Message& msg);
    BS2OStream& operator<< (const BS2ItemHeader& itemhead);
    bool isPrimary();
    bool isWait();

    int  blockHeader(BS2BlockHeader& bheader);
    BS2IStream * make() const;
    bool set(BS2Message * msg);
    void print(void * ar);
    void dump();

private:
    bool setList(BS2Items * items);

private:
    BS2StreamBuf m_sb;

};

#endif
