// $Id: BS2S1F0Message.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2S1F0Message.cpp
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

#include "BS2S1F0Message.h"

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
BS2S1F0Message const& BS2S1F0Message::operator=(BS2S1F0Message const& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2S1F0Message::operator=");
    if (this == &rhs)
        return *this;
    releaseAll();

    copy(rhs.items());
    return *this;
}

//
// *** End of File ***
