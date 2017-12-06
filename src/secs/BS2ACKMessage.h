// $Id: BS2ACKMessage.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ACKMessage.h
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

#ifndef BS2ACKMESSAGE_H
#define BS2ACKMESSAGE_H

#include "BS2Message.h"

enum
{
    ACK_OK = 0,                                   // Ack code
    ACK_NG = 0x01,                                // Nack code
};


/* ------------------------------------------------------------------------- */
class BEE_Export BS2ACKMessage : public BS2Message
{
public:
    BS2ACKMessage(int sfnum, BYTE ack, const BCHAR * iname);
    BS2ACKMessage(BS2ACKMessage const& rhs) : BS2Message(rhs.sf()) { }
    virtual ~BS2ACKMessage() { }

    BS2ACKMessage const& operator=(BS2ACKMessage const& rhs);
    int  result() const;
    void set(BYTE ack, const BCHAR * iname);

//
private:
};

/* ------------------------------------------------------------------------- */
class BS2S5F4Message : public BS2ACKMessage
{
public:
    BS2S5F4Message(BYTE ack)
        : BS2ACKMessage(SFCODE(5,4), ack, _TX("ACKC5")) { }
    virtual ~BS2S5F4Message() { }
};


//
// Include other Stream/Functions
//
#include "BS2S1F0Message.h"
#include "BS2ErrorMessage.h"

#endif /* BS2ACKMESSAGE_H */
