// $Id: BS2NoiseMessage.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2NoiseMessage.cpp
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

#include "BS2Binary.h"
#include "BS2Stream.h"
#include "BS2Item.h"
#include "BS2NoiseMessage.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2NoiseMessage::BS2NoiseMessage(BS2IStream * ibuf): BS2Message(SFCODE(0,0))
{
    BS2Atom * atom = new BS2Binary((BYTE *)ibuf->gtop(), ibuf->size());
    add(BS2Item::factory(_TX("ABS"), atom));
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
BS2NoiseMessage const& BS2NoiseMessage::operator=(BS2NoiseMessage const& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2NoiseMessage::operator=");
    if (this == &rhs)
        return *this;
    releaseAll();

    copy(rhs.items());
    return *this;
}

//
// *** End of File ***
