// $Id$

//=============================================================================
/**
 *  @file    BS2Peer.h
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

#ifndef BS2PEER_H
#define BS2PEER_H

#include "BS2Socket.h"

//-----------------------------------------------------------------------------
// SECS Socket Driver
//-----------------------------------------------------------------------------
class BEE_Export BS2Peer: public BS2Socket
{

friend void  SocketReadThreadProc(void * parm);
friend class BS2SessionTimer;

// constructor/destoructor
public:
    BS2Peer();
    virtual ~BS2Peer();

    virtual int  initialize(DeviceParameter * parm, BS2Device * device);
    virtual int  open();

    void peer(BS2Peer * peer) { m_peer = peer; }
    BS2Peer * peer()          { return m_peer; }
//
protected:
    virtual int  protocol(BYTE * readbuf, int bytes_read);

// variables
protected:
    BS2Peer * m_peer;
};

#endif
