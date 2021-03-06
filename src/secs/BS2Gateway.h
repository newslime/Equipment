// $Id$

//=============================================================================
/**
 *  @file    BS2Gateway.h
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

#ifndef BS2GATEWAY_H
#define BS2GATEWAY_H

#include "beesecs.h"
#include "BS2Device.h"
#include "BS2Message.h"

//-----------------------------------------------------------------------------
//
// Gateway Device Module
//
//-----------------------------------------------------------------------------
class BEE_Export BS2Gateway: public BS2Device
{
public:
    BS2Gateway();
    ~BS2Gateway();

    virtual int initialize(DeviceParameter * parm);
    virtual int disconnected();

    int  peerOpen();

protected:

};

#endif
