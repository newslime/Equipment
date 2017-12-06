/* $Id: bee.h,v 1.10 2004/08/14 14:48:42 fukasawa Exp $ */

/**
 *  @file    bee.h
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

#ifndef BEE_H
#define BEE_H

/*---------------------------------------------------------------------------*/
/*  system header files                                                      */
/*---------------------------------------------------------------------------*/
#if defined(_DEBUG) && defined(WIN32) && defined(LEAK_CHECK)
#define _CRTDBG_MAP_ALLOC
#endif

#include <stdio.h>                      /* try not to #include stdio.h twice */
#include <stdlib.h>
#if defined(_DEBUG) && defined(WIN32) && defined(LEAK_CHECK)
#include <crtdbg.h>
#endif
#include <ctype.h>
#include <limits.h>
#ifdef _MSC_VER
#include <tchar.h>
#else
typedef char TCHAR;
#define _T(x)        x

#define _stprintf  sprintf
#define _stscanf   scanf
#define _tcscat    strcat
#define _tcschr    strchr
#define _tcsstr    strstr
#define _tcscmp    strcmp
#define _tcscpy    strcpy
#define _tcsdup    strdup
#define _tcsftime  strftime
#define _tcslen    strlen
#define _tcsncpy   strncpy
#define _tcsrchr   strrchr
#define _tcstol    strtol
#define _tcstoul   strtoul
#define _tcstod    strtod
#define _tprintf   printf
#define _ttoi      atoi
#define _vtprintf   vprintf
#define _vstprintf  vsprintf
#define _vsntprintf vsnprintf
#define _sntprintf  snprintf

#endif

#ifdef __GNUC__
/* disable warning for undefined cuserid(); */
#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif
#include <unistd.h>
#endif


/*****************************************************************************/
/*                                                                           */
/*  Symbol                                                                   */
/*                                                                           */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/*  Define data types                                                        */
/*---------------------------------------------------------------------------*/
#if !defined(_MSC_VER) && !defined(VOID)
#define VOID    void
//typedef signed char     CHAR;
typedef signed long     LONG;
typedef signed short    SHORT;
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef UCHAR
typedef signed int      INT;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef void *          PTR;
#endif
#ifndef ULONG
typedef unsigned long   ULONG;
#endif

#ifndef BOOLEAN
typedef unsigned char   BOOLEAN;
typedef short           SHORTINT;
typedef long            LONGINT;
typedef unsigned short  SHORTCARD;
typedef unsigned long   LONGCARD;
typedef LONGCARD        CARDINAL;
typedef double          LONGREAL;
typedef LONGINT         INTEGER;
typedef float           REAL;
#endif

#ifdef uchar
typedef unsigned char   uchar;
typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;
#endif

#if _MSC_VER
typedef __int64             LONGLONG;
typedef unsigned __int64    ULONGLONG;
#elif __GNUC__
typedef long long           LONGLONG;
typedef unsigned long long  ULONGLONG;
#else
#error no support 64 bit data type; // LongLong ?
#endif

#ifdef BITSET
typedef unsigned long   BITSET;
typedef char * ADDRESS;
typedef int  (*PFI)();                  /* pointer to function returning int */
typedef void (*PFV)();                  /* pointer to function with no return*/

#endif

/*---------------------------------------------------------------------------*/
/*  redefine global tokens                                                    */
/*---------------------------------------------------------------------------*/
#define GLOBAL
#ifndef EXTERN
#define EXTERN       extern
#endif
#define PROTOTYPE    extern

/*----------------------------------------------------------------------------*/
/*  limit values                                                              */
/*----------------------------------------------------------------------------*/
#ifdef MSDOS
#define MAX_INT       32767
#define MAX_UINT      0xFFFF
#define MAX_INTSTR    "32767"
#define MAX_UINTSTR   "65535"
#else
#define MAX_INT       2147483647
#define MAX_UINT      0xFFFFFFFF
#define MAX_INTSTR    "2147483647"
#define MAX_UINTSTR   "4294967295"
#endif

#define MAX_SHORT     32767
#define MAX_LONG      2147483647
#define MAX_USHORT    0xFFFF
#define MAX_ULONG     0xFFFFFFFF
#define MAX_SHORTCARD 0xFFFF
#define MAX_LONGCARD  0xFFFFFFFF
#define MAX_SHORTSTR  "32767"
#define MAX_LONGSTR   "2147483647"
#define MAX_USHORTSTR "65535"
#define MAX_ULONGSTR  "4294967295"
#define MIN_REAL      ((REAL)1.40129846432481707e-45)
#define MAX_REAL      ((REAL)3.40282346638528860e+38)
#define MIN_LONGREAL  4.94065645841246544e-324
#define MAX_LONGREAL  1.79769313486231470e+308
#define MAX_LLSTR     "9223372036854775807"

/*****************************************************************************/
/*  Define ASCII codes                                                       */
/*****************************************************************************/
#ifdef WIN32
#define DIR_SEPARATOR_STRING  _TX("\\")
#define DIR_SEPARATOR_CHAR    '\\'
#else
#define DIR_SEPARATOR_STRING  _TX("/")
#define DIR_SEPARATOR_CHAR    '/'
#endif

#define TAB          '\t'       /* C definition of ASCII tab                 */
#define NEWLINE      '\n'       /* C definition of ASCII newline             */
#define BACKSPACE    '\b'       /* C definition of ASCII backspace           */
#define SPACE        ' '        /* white space character                     */

#define ESC          0x1B       /* Standard definition ASCII escape          */
#define BS           0x08       /* Standard definition ASCII backspace       */
#define CR           0x0D       /* Standard definition ASCII carriage return */
#define FF           0x0C       /* Standard definition ASCII form feed       */
#define BEL          0x07       /* Standard definition ASCII bell            */
#define HT           0x09       /* Standard definition ASCII horizontal tab  */
#define LF           0x0A       /* Standard definition ASCII line feed       */
#define VT           0x0B       /* Standard definition ASCII virtical tab    */
#define SUB          0x1A
#define RS           0x1E

/*****************************************************************************/
/*                                                                           */
/*  function macros                                                          */
/*                                                                           */
/*****************************************************************************/

#ifndef max
#define max(a, b)              ((a) >= (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b)              ((a) <= (b) ? (a) : (b))
#endif
#ifndef Max
#define Max(a, b)              ((a) >= (b) ? (a) : (b))
#endif
#ifndef Min
#define Min(a, b)              ((a) <= (b) ? (a) : (b))
#endif
#if 0
#ifndef MAX
#define MAX(a, b)              ((a) >= (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)              ((a) <= (b) ? (a) : (b))
#endif
#ifndef abs
#define abs(n)                 ((n) > 0 ? (n) : -(n))
#endif
#ifndef ABS
#define ABS(n)                 ((n) > 0 ? (n) : -(n))
#endif
#endif

/* word operation */
#define byte_high(a)           (((a) >> 8) & 0x00FF)
#define byte_low(a)            ((a) & 0x00FF)
#define HIGH(c)                (((c) >> 8) & 0x00FF)
#define LOW(c)                 ((c) & 0x00FF)
#define high(c)                (((c) >> 8) & 0x00FF)
#define low(c)                 ((c) & 0x00FF)
#define btow(h,l)              (((WORD)((BYTE)(h)) << 8) | (BYTE)(l))

/* long word operation  */
#define word_high(a)           (((a) >> 16) & 0xFFFF)
#define word_low(a)            ((a) & 0xFFFF)

/* convert byte and integer */
#ifdef _M_IX86
#define BUF_TO_SHORT(a,b)      (((USHORT)(b) << 8) | ((USHORT)(a) & 0xFF))
#define SHORT_TO_BUF(w,a,b)    (*((BYTE *)(b)) = ((USHORT)(w) >> 8) & 0xFF, \
                                *((BYTE *)(a)) = (USHORT)(w) & 0xFF)
#define BUF_TO_LONG(a,b,c,d)   (((ULONG)(d) << 24) | (((ULONG)(c) << 16) & 0xFF0000) | \
                                (((ULONG)(b) << 8) & 0xFF00) | ((ULONG)(a) & 0xFF))
#define LONG_TO_BUF(l,a,b,c,d) (*((BYTE *)(d)) = (BYTE)((ULONG)(l) >> 24), \
                                *((BYTE *)(c)) = (BYTE)((ULONG)(l) >> 16), \
                                *((BYTE *)(b)) = (BYTE)((ULONG)(l) >> 8 ), \
                                *((BYTE *)(a)) = (BYTE)((ULONG)(l) & 0xFF))
#else
#define BUF_TO_SHORT(a,b)      (((USHORT)(a) << 8) | ((USHORT)(b) & 0xFF))
#define SHORT_TO_BUF(w,a,b)    (*((BYTE *)(a)) = (((USHORT)(w)) >> 8) & 0xFF, \
                                *((BYTE *)(b)) = ((USHORT)(w)) & 0xFF)
#define BUF_TO_LONG(a,b,c,d)   (((ULONG)(a) << 24) | \
                                (((ULONG)(b) << 16) && 0xFF0000) | \
                                (((ULONG)(c) << 8) & 0xFF00) | ((ULONG)(d) & 0xFF))
#define LONG_TO_BUF(l,a,b,c,d) (*((BYTE *)(a)) = (BYTE)((ULONG)(l) >> 24), \
                                *((BYTE *)(b)) = (BYTE)((ULONG)(l) >> 16), \
                                *((BYTE *)(c)) = (BYTE)((ULONG)(l) >> 8), \
                                *((BYTE *)(d)) = (BYTE)((ULONG)(l) & 0xFF))
#endif

#define BUFPTR_TO_SHORT(p)     BUF_TO_SHORT(*((BYTE *)p), *(((BYTE *)p)+1))
#define SHORT_TO_BUFPTR(l, p)  SHORT_TO_BUF((l), ((BYTE *)p), (((BYTE *)p)+1))
#define BUFPTR_TO_LONG(p)      BUF_TO_LONG(*(((BYTE *)p)+0), *(((BYTE *)p)+1),   \
                                           *(((BYTE *)p)+2), *(((BYTE *)p)+3))
#define LONG_TO_BUFPTR(l, p)   LONG_TO_BUF((l), ((BYTE *)p), (((BYTE *)p)+1),    \
                                                (((BYTE *)p)+2), (((BYTE *)p)+3))

#define BCD_TO_BYTE(x)         (((((x) >> 4)  & 0xF) * 10)  + ((x) & 0xF))
#define BCD_TO_SHORT(x)        (((((x) >> 12) & 0xF) * 1000) +                \
                                ((((x) >> 8)  & 0xF) * 100) +                 \
                                ((((x) >> 4)  & 0xF) * 10)  + ((x) & 0xF))
#define BYTE_TO_BCD(x)         (((((x) % 100)   / 10)   << 4) + ((x) % 10))
#define SHORT_TO_BCD(x)        ((((x)          / 1000) << 12) +               \
                                ((((x) % 1000) / 100)  << 8) +                \
                                ((((x) % 100)  / 10)   << 4) + ((x) % 10))

/* check even and odd  */
#define is_odd(x)              ((x) & 1)
#define is_even(x)             (!((x) & 1))
#define ODD(x)                 (((x) & 01))

#define bound_word(p)          ((((p) + 1) / 2) * 2)
#define bound_long(p)          ((((p) + 1) / 4) * 4)

#define EQUALSTR(s, c)        ((*s == *c) && (strcmp(s, c) == 0))

/*---------------------------------------------------------------------------*/
/*  define boolean                                                           */
/*---------------------------------------------------------------------------*/
#ifndef TRUE
#define TRUE         1
#endif
#ifndef FALSE
#define FALSE        0
#endif
#ifndef SUCCESS
#define SUCCESS      0
#endif

#ifndef OK
#define OK           0
#endif
#ifndef NG
#define NG           1
#endif

#ifndef YES
#define YES          1
#endif
#ifndef NO
#define NO           0
#endif

#ifndef ON
#define ON           1
#endif
#ifndef OFF
#define OFF          0
#endif

#ifndef FAILURE
#define FAILURE      (-1)
#endif

#define BEE_SUCCESS  0
#define BEE_ERROR    (-1)

#ifndef LOOP
#define LOOP    for(;;)
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN     1234
#define LITTLE_ENDIAN  4321
#endif
#if defined(_M_IX86) || defined(__386__) || defined(i386)
#define ENDIAN         LITTLE_ENDIAN
#else
#define ENDIAN         BIG_ENDIAN
#endif

#endif  /* BEE_H */
