// $Id: b_command.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_command.cpp
 *
 *  @author  Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2001-2004 BEE Co.,Ltd. All rights reserved.
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

#include "b_command.h"

//-----------------------------------------------------------------------------
// Get parameter value by name.
//-----------------------------------------------------------------------------
bool b_command::getValue(const string& parname, b_value& retval)
{
    for (size_t i = 0; i < m_params.size(); i++)
    {
        if (m_params[i].m_pname == parname)
        {   // found the parameter
            retval = m_params[i].m_pval;
            return true;
        }
    }
    return false;
}

