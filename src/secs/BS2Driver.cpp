// $Id: BS2Driver.cpp,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Driver.cpp
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

#include "BS2Driver.h"
#include "BS2Device.h"

//-----------------------------------------------------------------------------
// Constructor/Destoructor
//-----------------------------------------------------------------------------
BS2Driver::BS2Driver()
        : m_deviceId(0), m_sourceId(0), m_sendbuf(NULL), m_sendbufSize(0),
          m_recvbuf(NULL), m_recvbufSize(0), m_device(NULL), m_hexdump(false)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Driver::BS2Driver");
    m_open = false;
    m_slave = false;

	m_bs2driver_loop = 0;
}

//-----------------------------------------------------------------------------
BS2Driver::~BS2Driver()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Driver::~BS2Driver");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int BS2Driver::initialize(DeviceParameter * parm, BS2Device * device)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::initialize");

    m_deviceId = parm->m_deviceId;
    m_sourceId = parm->m_sourceId;
    m_slave = (parm->m_slave != 0) ? true : false;
    m_device = device;
    if (parm->m_dump)
    {
        m_hexdump = true;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Open
//-----------------------------------------------------------------------------
int BS2Driver::open()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::open");

    return 0;
}

//-----------------------------------------------------------------------------
// Close
//-----------------------------------------------------------------------------
int BS2Driver::close()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::close");

    return 0;
}

//-----------------------------------------------------------------------------
// Enable/Disable
//-----------------------------------------------------------------------------
#if 0
int BS2Driver::enable()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::enable");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("driver is not opened \n")));
    }
    m_disable = 0;

    return 0;
}

//-----------------------------------------------------------------------------
int BS2Driver::disable()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::disable");

    if (m_open == false)
    {
        TRACE_ERROR((_TX("driver is not opened \n")));
        return -1;
    }
    m_disable = 1;

    return 0;
}
#endif
//-----------------------------------------------------------------------------
bool BS2Driver::isEnable() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::isEnable");

    return (m_disable == 0) ? true : false;
}

//-----------------------------------------------------------------------------
// Send
//-----------------------------------------------------------------------------
int BS2Driver::send(BS2TransactionInfo *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::send");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return -1;
}

//-----------------------------------------------------------------------------
// Receive
//-----------------------------------------------------------------------------
int BS2Driver::recv(BCHAR *, int)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::recv");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return -1;
}

//-----------------------------------------------------------------------------
// Receive
//-----------------------------------------------------------------------------
bool BS2Driver::canSend() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::canSend");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return false;
}

//-----------------------------------------------------------------------------
// Make Linktest Message
//-----------------------------------------------------------------------------
BS2OStream * BS2Driver::makeLinktestMessage()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Driver::makeLinktestMessage");

    TRACE_ERROR((_TX("sorry, not yet implement !\n")));

    return NULL;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Driver::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_BEGIN_DUMP, this));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_open = %d"), this->m_open));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_sendbuf = 0x%x"), this->m_sendbuf));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_sendbufSize = %d"), this->m_sendbufSize));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_recvbuf = 0x%x"), this->m_recvbuf));
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("\nm_recvbufSize = %d"), this->m_recvbufSize));
    ACE_DEBUG((LM_DEBUG, ACE_END_DUMP));
}

