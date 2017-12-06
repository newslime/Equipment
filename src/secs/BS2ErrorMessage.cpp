// $Id: BS2ErrorMessage.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ErrorMessage.cpp
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

#include "BS2ItemDictionary.h"
#include "BS2Binary.h"
#include "BS2ErrorMessage.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ErrorMessage::BS2ErrorMessage(int sfcode, BS2BlockHeader& header)
        : BS2Message(sfcode)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ErrorMessage::BS2ErrorMessage");

    m_wbit = false;

    BS2Item * mhead;
    BS2Binary * atom = new BS2Binary((BYTE *)&header, sizeof(BS2BlockHeader));
    if (FUNCCODE(sfcode) == 9)
    {
        mhead = BS2Item::factory(_TX("SHEAD"), atom);
    }
    else
    {
        mhead = BS2Item::factory(_TX("MHEAD"), atom);
    }
    add((BS2Item *)mhead);
}

//-----------------------------------------------------------------------------
BS2ErrorMessage::BS2ErrorMessage(int sfcode, BS2Message * srcmsg)
        : BS2Message(sfcode)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ErrorMessage::BS2ErrorMessage");
    BS2Item * mhead;
    BS2Binary * atom;
    BS2BlockHeader bheader;

    m_wbit = false;

    srcmsg->get(&bheader);
    atom = new BS2Binary((BYTE *)&bheader, sizeof(BS2BlockHeader));
    if (FUNCCODE(sfcode) == 9)
    {
        mhead = BS2Item::factory(_TX("SHEAD"), atom);
    }
    else
    {
        mhead = BS2Item::factory(_TX("MHEAD"), atom);
    }
    add((BS2Item *)mhead);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
BS2ErrorMessage const& BS2ErrorMessage::operator=(BS2ErrorMessage const& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ErrorMessage::operator=");
    if (this == &rhs)
        return *this;
    releaseAll();

    copy(rhs.items());
    return *this;
}

//
// *** End of File ***
