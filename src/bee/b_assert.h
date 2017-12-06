// $Id: b_assert.h,v 1.7 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_assert.h
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

#ifndef B_ASSERT_H
#define B_ASSERT_H

#include <string>
#include <stdexcept>
using namespace std;

#define _AsString(identifier) #identifier

#ifdef NDEBUG
#define BEEAssert(x)
#define BEEPanic(x)
#else
#define BEEPanic(x)  Panic<runtime_error>((x), __FILE__, __LINE__)
#define BEEAssert(x) Assert<runtime_error>((x), __FILE__, __LINE__, #x)

typedef runtime_error BEException;

#endif  /* _NO_DEBUG */

template<class T, class A>
inline void Assert(A assertion, char * fname, int lineNum, char * astr)
{
    if (! assertion)
    {
        char lbuf[16];
        string s;
        sprintf(lbuf, "(%d): ", lineNum);
        s += (strrchr(fname, DIR_SEPARATOR_CHAR) + 1);
        s += lbuf;
        s += "ASSERT(";
        s += astr;
        s += ")";
        throw T(s);
    }
}

template<class T>
inline void Panic(char * msg, char * fname, int linenum)
{
    char lbuf[16];
    string s = "Panic! ";
    sprintf(lbuf, "(%d): ", linenum);
    s += (strrchr(fname, DIR_SEPARATOR_CHAR) + 1);
    s += lbuf;
    s += msg;
    throw T(s);
}

#endif /* B_ASSERT_H */
