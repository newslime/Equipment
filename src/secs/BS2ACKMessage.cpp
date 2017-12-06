// $Id: BS2ACKMessage.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ACKMessage.cpp
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
#include "BS2ACKMessage.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ACKMessage::BS2ACKMessage(int sfcode, BYTE ack, const BCHAR * iname)
        : BS2Message(sfcode)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ACKMessage::BS2ACKMessage");
    BS2Item * ackItem;
    BS2Binary * atom = new BS2Binary(ack);
    ackItem = BS2Item::factory((BCHAR *)iname, atom);
    add(ackItem);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
BS2ACKMessage const& BS2ACKMessage::operator=(BS2ACKMessage const& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ACKMessage::operator=");
    if (this == &rhs)
        return *this;
    releaseAll();

    copy(rhs.items());
    return *this;
}

//-----------------------------------------------------------------------------
// Get ack code
//-----------------------------------------------------------------------------
int BS2ACKMessage::result() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ACKMessage::result");
    BS2Item * item = this->items().front();
    BS2Atom * atom = item->atom();
    BS2value val = *atom;
    return val.getInt();
}

//-----------------------------------------------------------------------------
void BS2ACKMessage::set(BYTE ack, const BCHAR * iname)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ACKMessage::set");
    BS2Item * ackItem;
    BS2Binary * atom = new BS2Binary(ack);
    ackItem = BS2Item::factory((BCHAR *)iname, atom);
    add(ackItem);
}

//
// *** End of File ***
