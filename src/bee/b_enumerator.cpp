// $Id: b_enumerator.cpp,v 1.1 2004/06/21 16:27:23 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_enumerator.cpp
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2004 BEE Co.,Ltd. All rights reserved.
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

#include "b_enumerator.h"

b_enumerator * b_enumerator::parse(const char * name, const char * str)
{
    b_enumerator * enum_ptr = new b_enumerator(name);
    char * enumstr = (char *)str;
    string token;
    int num = 0, cur = 0;
    while (*enumstr != '\0')
    {
        if (isspace(*enumstr))
        {
            enumstr++;
            continue;
        }
        if (*enumstr == '=')
        {
            enumstr++;
            while (*enumstr != '\0' && isspace(*enumstr))
                enumstr++;
            if (*enumstr == '\0')
                break;
            cur = strtol(enumstr, &enumstr, 0);
            if (cur >= num)
                num = cur;
        }
        else if (*enumstr == ',')
        {
            enum_ptr->add(token, cur);
            token.clear();
            num++;
            cur = num;
            enumstr++;
        }
        else
        {
            token.push_back(*enumstr);
            enumstr++;
        }
    }
    enum_ptr->add(token, cur);
    return enum_ptr;
}

