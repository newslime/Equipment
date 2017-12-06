// $Id: BS2S1F0Message.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2S1F0Message.h
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

#ifndef BS2S1F0MESSAGE_H
#define BS2S1F0MESSAGE_H

#include "BS2Message.h"

/* ------------------------------------------------------------------------- */
class BEE_Export BS2S1F0Message : public BS2Message
{
public:
    BS2S1F0Message() : BS2Message(SFCODE(1,0)) { }
    BS2S1F0Message(BS2S1F0Message const& rhs) : BS2Message(rhs.sf()) { }
    virtual ~BS2S1F0Message() { }

    BS2S1F0Message const& operator=(BS2S1F0Message const& rhs);

//
private:
};

/* ------------------------------------------------------------------------- */

#endif /* BS2S1F0MESSAGE_H */
