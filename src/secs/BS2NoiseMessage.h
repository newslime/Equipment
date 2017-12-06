// $Id: BS2NoiseMessage.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2NoiseMessage.h
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

#ifndef BS2NOISEMESSAGE_H
#define BS2NOISEMESSAGE_H

#include "BS2Message.h"

/* ------------------------------------------------------------------------- */
class BEE_Export BS2NoiseMessage : public BS2Message
{
public:
    BS2NoiseMessage() : BS2Message(SFCODE(0,0)) { }
    BS2NoiseMessage(BS2IStream * ibuf);
    BS2NoiseMessage(BS2NoiseMessage const& rhs) : BS2Message(rhs.sf()) { }
    virtual ~BS2NoiseMessage() { }
    virtual bool noise() const { return true; }

    BS2NoiseMessage const& operator=(BS2NoiseMessage const& proto);

//
private:
};

/* ------------------------------------------------------------------------- */

#endif /* BS2NOISEMESSAGE_H */
