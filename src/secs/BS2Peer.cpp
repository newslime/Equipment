// $Id$

//=============================================================================
/**
 *  @file    BS2Peer.cpp
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

#include "BS2Peer.h"

//-----------------------------------------------------------------------------
// Constructor/Destoructor
//-----------------------------------------------------------------------------
BS2Peer::BS2Peer(): BS2Socket()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Peer::BS2Peer");
    m_peer = NULL;
    m_gateway = true;
}

//-----------------------------------------------------------------------------
BS2Peer::~BS2Peer()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Peer::~BS2Peer");
}

//-----------------------------------------------------------------------------
// Initialize
//-----------------------------------------------------------------------------
int BS2Peer::initialize(DeviceParameter * config, BS2Device * device)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Peer::initialize");

    int result = BS2Socket::initialize(config, device);
    if (result < 0)
    {
        return result;
    }
    //
    //SocketParameter * parm = (SocketParameter *)config;
    //m_mode = parm->m_mode;
    //if (parm->m_mode == HSMS_MODE_ACTIVE)
    //{
    //}
    //else
    //{
    //}

    return result;
}

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------
int BS2Peer::open()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Peer::open");

    int result = BS2Socket::open();
    if (result < 0)
    {
        return result;
    }
    return result;
}

//-----------------------------------------------------------------------------
// HSMS protocol procedure
//-----------------------------------------------------------------------------
int BS2Peer::protocol(BYTE * dptr, int dataq)
{
    int result = BEE_SUCCESS;

    if (this->m_peer)
    {
        if (! this->m_peer->isOpened())
        {
            while (! (this->m_peer->isOpened() &&
                      this->getStatus() == HSMS_NOT_SELECTED))
            {
                ACE_Time_Value tv(0, 200 * 1000);
                ACE_OS::sleep(tv);
            }
        }
        this->m_peer->send((BCHAR *)dptr, dataq, true);
    }

#if defined(_DEBUG) && defined(WIN32) && defined(LEAK_CHECK)
    memdbg_begin("BS2Peer::protocol");
#endif
    result = this->BS2Socket::protocol(dptr, dataq);
    return result;
}

