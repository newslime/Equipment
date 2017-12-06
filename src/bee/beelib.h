/* $Id: beelib.h,v 1.7 2004/08/14 14:48:42 fukasawa Exp $ */

/**
 *  @file    beelib.h
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

#ifndef BEELIB_H
#define BEELIB_H

#include "bee.h"
#include "b_assert.h"
#include "BEE_Export.h"
#if defined(_DEBUG) && defined(WIN32) && defined(LEAK_CHECK)
#include "b_memdbg.h"
#endif

struct BEE_Export _FunctionTracer
{
    const BCHAR * m_funcname;

    _FunctionTracer(const BCHAR * func) : m_funcname(func) {}

    int  printf(BCHAR * fmt, ...);
    void error(BCHAR * fmt, ...);
    void debug(BCHAR * fmt, ...);
};

BEE_Export void del_space(BCHAR * bufptr);    /* delete white spaces */
BEE_Export void b_setTraceLevel(int level);
BEE_Export int  b_getTraceLevel();

BEE_Export int  b_printf(BCHAR * fmt, ...);
BEE_Export void b_error(BCHAR * fmt, ...);
BEE_Export void b_debug(BCHAR * fmt, ...);

#define TRACE_ERROR(X)  do { __func.error X ; } while(0)
#define TRACE_DEBUG(X)  do { __func.debug X ; } while(0)
//#define TRACE_ERROR(X)  do { b_error(_TX("%s: "), __func); b_error X ; } while(0)
//#define TRACE_DEBUG(X)  do { b_debug(_TX("%s: "), __func); b_debug X ; } while(0)

/* prototype memory dump procedure  */
#define BOUNDARY     0
#define NO_BOUNDARY  1
BEE_Export extern void mem_dump(BCHAR * startp, BCHAR * endp, ULONG base,
                                int mode = NO_BOUNDARY);

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

BEE_Export extern void hex_dump(BCHAR * startp, size_t size, string& hexstr,
                                int mode = (DUMP_WORD | DUMP_NEWLINE));
#endif

#ifdef _NO_DEBUG
#ifdef ACE_ACE_H
#define TRACE_FUNCTION(l,p)    BCHAR * __func = (p); ACE_UNUSED_ARG(__func)
#else
#define TRACE_FUNCTION(l,p)    BCHAR * __func = (p)
#endif
#define ACE_ERROR_RETURN(X, Y)
#else
#ifdef ACE_ACE_H
#define TRACE_FUNCTION(l,p)    \
        _FunctionTracer __func(p);   \
        ACE_UNUSED_ARG(__func); \
        if ((l) <= b_getTraceLevel()) { ACE_DEBUG((LM_DEBUG, _TX("%s\n"), _TX(p))); }
//#define TRACE_FUNCTION(l,p)    \
//        BCHAR * __func = (p);   \
//        ACE_UNUSED_ARG(__func); \
//        if ((l) <= b_getTraceLevel()) { ACE_DEBUG((LM_DEBUG, _TX("%s\n"), _TX(p))); }
#else
#define TRACE_FUNCTION(l,p)    \
        BCHAR * __func = (p);   \
        if ((l) <= b_getTraceLevel()) b_printf(p)
#endif
#endif  /* _NO_DEBUG */

enum
{
    TRL_MUST = 1,
    TRL_HIGH = 20,
    TRL_VIEW = 40,
    TRL_MIDDLE = 50,
    TRL_LOW = 80,
    TRL_CONSTRUCT = 99,
    TRL_IGNORE = 100,
};

#endif  /* BEELIB_H */
