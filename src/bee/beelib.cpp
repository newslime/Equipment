// $Id: beelib.cpp,v 1.9 2004/08/14 14:48:42 fukasawa Exp $

//=============================================================================
/**
 *  @file    beelib.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include "b_ace.h"
#include "beelib.h"

#define PRINT_BUFSIZE  (ACE_MAXLOGMSGLEN - 256)

bool _ARG_CHECK = true;

/*---------------------------------------------------------------------------*/
/* Panic and Assert function for C language                                  */
/*---------------------------------------------------------------------------*/
BEE_Export void _Panic(BCHAR * msg, BCHAR * fname, int lineNum)
{
    Panic<runtime_error>(msg, fname, lineNum);
}

BEE_Export void _Assert(BCHAR * fname, int lineNum, BCHAR * astr)
{
    if (_ARG_CHECK)
    {
        BCHAR lbuf[16];
        string s;
        sprintf(lbuf, "(%d): ", lineNum);
        s += (strrchr(fname, DIR_SEPARATOR_CHAR) + 1);
        s += lbuf;
        s += "ASSERT(";
        s += astr;
        s += ")";
        throw runtime_error(s);
    }
}

/*---------------------------------------------------------------------------*/
/* Trace level code                                                          */
/*---------------------------------------------------------------------------*/
static int _bee_traceLevel = TRL_VIEW;

void b_setTraceLevel(int level)
{
    _bee_traceLevel = level;
}

int b_getTraceLevel()
{
    return _bee_traceLevel;
}

/*---------------------------------------------------------------------------*/
/* bit pattern / num                                                         */
/*---------------------------------------------------------------------------*/
unsigned int BitTable[32] =
{
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

unsigned int bit_encode(unsigned int bit)
{
    UINT i;
    for (i = 0; i < 32; i++)
    {
        if (bit == BitTable[i])
            break;
    }
    return ((i >= 32) ? 0xFFFFFFFF: i);
}

unsigned int bit_decode(unsigned int bitCode)
{
    if (bitCode > 31)
        throw out_of_range("bit_decode()");
    return BitTable[bitCode];
}


/*---------------------------------------------------------------------------*/
/* delete blank code                                                         */
/*---------------------------------------------------------------------------*/
void del_space(BCHAR * bufp)
{
    BCHAR c;
    BCHAR *srcp, *dstp;

    if (bufp)
    {
        /* */
        srcp = dstp = bufp;
        while ((c = *dstp) == ' ' || c == '\t' || c == '\n')
            dstp++;
        while (*dstp)
            *srcp++ = *dstp++;
        *srcp = '\0';

        /*  */
        srcp = bufp + _tcslen((BCHAR const * )bufp);
        while (srcp != bufp)
        {
            if ((c = *(srcp - 1)) != ' ' && c != '\t' && c != '\n')
                break;
            else
                srcp--;
        }
        *srcp = '\0';
    }
    return ;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* hex dump                                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef BOUNDARY
#define BOUNDARY     0
#define NO_BOUNDARY  1
#endif

BCHAR lower_code[] = _TX("0123456789abcdef");
BCHAR upper_code[] = _TX("0123456789ABCDEF");

#define BYTE_FORMAT
#ifndef BYTE_FORMAT

#define HEX_POINT      0
#define SEPARATE_POINT 44
#define CHAR_POINT     47
static BCHAR linebuf[] =
    _TX("1234 1234  1234 1234  1234 1234  1234 1234  : 1234 1234 1234 1234\n");

/*---------------------------------------------------------------------------*/
/* word format                                                               */
/*---------------------------------------------------------------------------*/
void mem_dump(BCHAR * startp, BCHAR * endp, ULONG base, int mode)
{
    BCHAR * curptr;
    UINT    dumpnt;
    BCHAR   c;
    BCHAR * hexp;
    BCHAR * charp;
    BCHAR * hexcodep;

    if (mode == BOUNDARY)
    {   /*  */
        curptr = (BCHAR *)((ULONG)startp & ~0x0F);
        base &= ~0x0F;
    }
    else
        curptr = startp;

    hexcodep = (BCHAR *)lower_code;     /*  */

/*---------------------------------------------------------------------------*/
/* dump hex data                                                             */
/*---------------------------------------------------------------------------*/
    for ( ; curptr < endp; curptr += 16)
    {
        hexp = (BCHAR *)(linebuf + HEX_POINT);
        charp = (BCHAR *)(linebuf + CHAR_POINT);

        for (dumpnt = 0; *(hexp + dumpnt) != '\n'; dumpnt++)
            *(hexp + dumpnt) = ' ';
        *(hexp + SEPARATE_POINT) = ':';

        for (dumpnt = 0; dumpnt < 16; dumpnt++)
        {
            c = *(curptr + dumpnt);
            if (dumpnt != 0 && (dumpnt & 0x01) == 0)
                hexp++;
            if (dumpnt != 0 && (dumpnt & 0x03) == 0)
            {
                hexp++;
                charp++;
            }
/*---------------------------------------------------------------------------*/
/* dump character                                                            */
/*---------------------------------------------------------------------------*/
            if ((curptr + dumpnt) < startp || (curptr + dumpnt) >= endp)
            {
                hexp += 2;
                charp++;
            }
            else
            {
                *hexp++ = hexcodep[(c >> 4) & 0x0F];
                *hexp++ = hexcodep[c & 0x0F];
                if (c < ' ' || c > '~')
                    *charp++ = '.';
                else
                    *charp++ = c;
            }
        }
        b_printf("%8lx    %s", base, linebuf);
        base += 16;
    }
}
#else

#define HEX_POINT      0
#define SEPARATE_POINT 50
#define CHAR_POINT     52
static BCHAR linebuf[] =
    _TX("12 12 12 12 12 12 12 12  12 12 12 12 12 12 12 12 : 12345678 12345678 \n");

/*---------------------------------------------------------------------------*/
/* byte format                                                               */
/*---------------------------------------------------------------------------*/
void mem_dump(BCHAR * startp, BCHAR * endp, ULONG base, int mode)
{
    BCHAR * curptr;
    UINT   dumpnt;
    BCHAR   c;
    BCHAR * hexp;
    BCHAR * charp;
    BCHAR * hexcodep;

    if (mode == BOUNDARY)
    {
        curptr = (BCHAR *)((unsigned long)startp & ~0x0F);
        base &= ~0x0F;
    }
    else
        curptr = startp;

    hexcodep = (BCHAR *)lower_code;

/*---------------------------------------------------------------------------*/
/* dump hex data                                                             */
/*---------------------------------------------------------------------------*/
    for ( ; curptr < endp; curptr += 16)
    {
        hexp = (BCHAR *)(linebuf + HEX_POINT);
        charp = (BCHAR *)(linebuf + CHAR_POINT);

        for (dumpnt = 0; *(hexp + dumpnt) != '\n'; dumpnt++)
            *(hexp + dumpnt) = ' ';
        *(hexp + SEPARATE_POINT) = ':';

        for (dumpnt = 0; dumpnt < 16; dumpnt++)
        {
            c = *(curptr + dumpnt);
            hexp++;
            if (dumpnt != 0 && (dumpnt & 0x07) == 0)
            {
                hexp++;
                charp++;
            }
/*---------------------------------------------------------------------------*/
/* dump character                                                            */
/*---------------------------------------------------------------------------*/
            if ((curptr + dumpnt) < startp || (curptr + dumpnt) >= endp)
            {
                hexp += 2;
                charp++;
            }
            else
            {
                *hexp++ = hexcodep[(c >> 4) & 0x0F];
                *hexp++ = hexcodep[c & 0x0F];
                if (c < ' ' || c > '~')
                    *charp++ = '.';
                else
                    *charp++ = c;
            }
        }
        b_printf(_TX("%08lx %s"), base, linebuf);
        base += 16;
    }
}
#endif

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* hex dump                                                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
static void _dumpword(BCHAR * startp, size_t size, string& hexstr,
                      BCHAR * hexcodep, int mode);
static void _dumpbyte(BCHAR * startp, size_t size, string& hexstr,
                      BCHAR * hexcodep, int mode);

#ifndef DUMP_BYTE
#define DUMP_BYTE       0x0000
#define DUMP_WORD       0x0001
#define DUMP_LOWER      0x0000
#define DUMP_UPPER      0x0002
#define DUMP_OFFSET     0x1000
#define DUMP_CHAR       0x2000
#define DUMP_CONT       0x4000
#define DUMP_NEWLINE    0x8000
#define DUMP_TCL        (DUMP_OFFSET | DUMP_CONT | DUMP_NEWLINE)
#endif

void hex_dump(BCHAR * startp, size_t size, string& hexstr, int mode)
{
    BCHAR * hexcodep = (mode & DUMP_UPPER) ? upper_code : lower_code;
    if (mode & DUMP_WORD)
    {
        _dumpword(startp, size, hexstr, hexcodep, mode);
    }
    else
    {
        _dumpbyte(startp, size, hexstr, hexcodep, mode);
    }
}


#define WORD_HEX_POINT      0
#define WORD_SEPARATE_POINT 43
#define WORD_CHAR_POINT     45
static BCHAR line_word[] =
    _TX("1234 1234  1234 1234  1234 1234  1234 1234  : 1234 1234 1234 1234");

/*---------------------------------------------------------------------------*/
/* word format                                                               */
/*---------------------------------------------------------------------------*/
void _dumpword(BCHAR * startp, size_t size, string& hexstr, BCHAR * hexcodep,
               int mode)
{
    BCHAR * endp = startp + size;
    BCHAR * curptr;
    UINT   dumpnt;
    BCHAR   c;
    BCHAR * hexp;
    BCHAR * charp;
    BCHAR linebuf[128];
    long base = 0;                    // offset position

    hexstr = _TX("");
    curptr = startp;

    strcpy(linebuf, line_word);
    //------------------------------------------------------------------
    // dump hex data
    //------------------------------------------------------------------
    for ( ; curptr < endp; curptr += 16)
    {
        hexp = (BCHAR *)(linebuf + WORD_HEX_POINT);
        charp = (BCHAR *)(linebuf + WORD_CHAR_POINT);

        for (dumpnt = 0; *(hexp + dumpnt) != '\0'; dumpnt++)
            *(hexp + dumpnt) = ' ';
        *(hexp + WORD_SEPARATE_POINT) = ':';

        for (dumpnt = 0; dumpnt < 16; dumpnt++)
        {
            c = *(curptr + dumpnt);
            if (dumpnt != 0 && (dumpnt & 0x01) == 0)
                hexp++;
            if (dumpnt != 0 && (dumpnt & 0x03) == 0)
            {
                hexp++;
                charp++;
            }
            //----------------------------------------------------------
            // dump character
            //----------------------------------------------------------
            if ((curptr + dumpnt) < startp || (curptr + dumpnt) >= endp)
            {
                hexp += 2;
                charp++;
            }
            else
            {
                *hexp++ = hexcodep[(c >> 4) & 0x0F];
                *hexp++ = hexcodep[c & 0x0F];
                if (c < ' ' || c > '~')
                    *charp++ = '.';
                else
                    *charp++ = c;
            }
        }

        //--------------------------------------------------------------
        // formatting
        //--------------------------------------------------------------
        if (! (mode & DUMP_CHAR))
        {   // Cut printed character code
            linebuf[WORD_SEPARATE_POINT] = '\0';
        }
        if (mode & DUMP_OFFSET)
        {
            BCHAR buf[128];
            sprintf(buf, "%08lx ", base);
            hexstr += buf;
        }

        hexstr += linebuf;                        // copy hex data

        if ((mode & DUMP_CHAR) || (mode & DUMP_NEWLINE))
        {
            if (mode & DUMP_CONT)
            {
                hexstr += DIR_SEPARATOR_STRING;
            }
            hexstr += _TX("\n");
        }

        base += 8;
    }
}


#define BYTE_HEX_POINT      0
#define BYTE_SEPARATE_POINT 49
#define BYTE_CHAR_POINT     51
static BCHAR line_byte[] =
    _TX("12 12 12 12 12 12 12 12  12 12 12 12 12 12 12 12 : 12345678 12345678");

/*---------------------------------------------------------------------------*/
/* byte format                                                               */
/*---------------------------------------------------------------------------*/
void _dumpbyte(BCHAR * startp, size_t size, string& hexstr, BCHAR * hexcodep,
               int mode)
{
    BCHAR * endp = startp + size;
    BCHAR * curptr;
    UINT   dumpnt;
    BCHAR   c;
    BCHAR * hexp;
    BCHAR * charp;
    BCHAR linebuf[128];
    long base = 0;                    // offset position

    hexstr = "";
    curptr = startp;

    strcpy(linebuf, line_byte);
    //------------------------------------------------------------------
    // dump hex data
    //------------------------------------------------------------------
    for ( ; curptr < endp; curptr += 16)
    {
        hexp = (BCHAR *)(linebuf + BYTE_HEX_POINT);
        charp = (BCHAR *)(linebuf + BYTE_CHAR_POINT);

        for (dumpnt = 0; *(hexp + dumpnt) != '\0'; dumpnt++)
            *(hexp + dumpnt) = ' ';
        *(hexp + BYTE_SEPARATE_POINT) = ':';

        for (dumpnt = 0; dumpnt < 16; dumpnt++)
        {
            c = *(curptr + dumpnt);
            hexp++;
            if (dumpnt != 0 && (dumpnt & 0x07) == 0)
            {
                hexp++;
                charp++;
            }
            //----------------------------------------------------------
            // dump character
            //----------------------------------------------------------
            if ((curptr + dumpnt) < startp || (curptr + dumpnt) >= endp)
            {
                hexp += 2;
                charp++;
            }
            else
            {
                *hexp++ = hexcodep[(c >> 4) & 0x0F];
                *hexp++ = hexcodep[c & 0x0F];
                if (c < ' ' || c > '~')
                    *charp++ = '.';
                else
                    *charp++ = c;
            }
        }
        //--------------------------------------------------------------
        // formatting
        //--------------------------------------------------------------
        if (! (mode & DUMP_CHAR))
        {   // Cut printed character code
            linebuf[BYTE_SEPARATE_POINT] = '\0';
        }
        if (mode & DUMP_OFFSET)
        {
            BCHAR buf[128];
            sprintf(buf, _TX("%08lx "), base);
            hexstr += buf;
        }

        hexstr += linebuf;                        // copy hex data

        if ((mode & DUMP_CHAR) || (mode & DUMP_NEWLINE))
        {
            if (mode & DUMP_CONT)
            {
                hexstr += DIR_SEPARATOR_STRING;
            }
            hexstr += _TX("\n");
        }

        base += 16;
    }
}

/*---------------------------------------------------------------------------*/
/* printf wrapper                                                            */
/*---------------------------------------------------------------------------*/
int _ACE_Log_Output = 0;

int b_printf(BCHAR *fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

#ifdef ACE_ACE_H
    BCHAR buffer[PRINT_BUFSIZE+1];
    if (_ACE_Log_Output == 1)
    {
        _vsntprintf(buffer, PRINT_BUFSIZE, fmt, arglist);
        ACE_DEBUG((LM_DEBUG, buffer));
    }
    else
    {
        _vtprintf((const BCHAR *)fmt, arglist);
    }
#else
    _vtprintf((const BCHAR *)fmt, arglist);
#endif

    return 1;
}

/*---------------------------------------------------------------------------*/
int b_sprintf(void * ar, BCHAR *fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

#ifdef _TCL              // defined by tcl.h
    BCHAR buffer[PRINT_BUFSIZE+1];
    Tcl_Interp * interp = (Tcl_Interp *)ar;
    _vsntprintf(buffer, PRINT_BUFSIZE, fmt, arglist);
    Tcl_AppendResult(interp, buffer, NULL);
#else
    _vstprintf((BCHAR *)ar, (const BCHAR *)fmt, arglist);
#endif
    return 1;
}

/*---------------------------------------------------------------------------*/
void b_abort(BCHAR *fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

#ifdef ACE_ACE_H
    BCHAR buffer[PRINT_BUFSIZE+1];
    _vsntprintf(buffer, PRINT_BUFSIZE, fmt, arglist);
    ACE_DEBUG((LM_DEBUG, buffer));
#else
    _vtprintf((const BCHAR *)fmt, arglist);
#endif

    exit(-1);
}

/*---------------------------------------------------------------------------*/
//
void b_error(BCHAR * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

#ifdef ACE_ACE_H
    BCHAR buffer[PRINT_BUFSIZE+1];
    _vsntprintf(buffer, PRINT_BUFSIZE, fmt, arglist);

    ACE_ERROR((LM_ERROR, buffer));
#else
    _vtprintf((const BCHAR *)fmt, arglist);
#endif
    return ;
}

/*---------------------------------------------------------------------------*/
//
void b_debug(BCHAR * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

#ifdef ACE_ACE_H
    BCHAR buffer[PRINT_BUFSIZE+1];
    _vsntprintf(buffer, PRINT_BUFSIZE, fmt, arglist);

    ACE_DEBUG((LM_DEBUG, buffer));
#else
    _vtprintf((const BCHAR *)fmt, arglist);
#endif
    return ;
}

#ifdef ACE_ACE_H
//-----------------------------------------------------------------------------
// Trace function class
//-----------------------------------------------------------------------------
int _FunctionTracer::printf(BCHAR * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    if (_ACE_Log_Output == 1)
    {
        BCHAR buffer[PRINT_BUFSIZE+1];
        _stprintf(buffer, _TX("%s: "), m_funcname);
        size_t q = _tcslen(buffer);

        _vsntprintf(buffer + q, PRINT_BUFSIZE - q, fmt, arglist);
        ACE_DEBUG((LM_DEBUG, buffer));
    }
    else
    {
        _tprintf("%s: ", m_funcname);
        _vtprintf(fmt, arglist);
    }
    return 1;
}

//
void _FunctionTracer::error(BCHAR * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    BCHAR buffer[PRINT_BUFSIZE+1];
    _stprintf(buffer, _TX("%s: "), m_funcname);
    size_t q = _tcslen(buffer);

    _vsntprintf(buffer + q, PRINT_BUFSIZE - q, fmt, arglist);
    ACE_ERROR((LM_ERROR, buffer));
    return ;
}

//
void _FunctionTracer::debug(BCHAR * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    BCHAR buffer[PRINT_BUFSIZE+1];
    _stprintf(buffer, _TX("%s: "), m_funcname);
    size_t q = _tcslen(buffer);

    _vsntprintf(buffer + q, PRINT_BUFSIZE - q, fmt, arglist);
    ACE_DEBUG((LM_DEBUG, buffer));
    return ;
}
#endif

