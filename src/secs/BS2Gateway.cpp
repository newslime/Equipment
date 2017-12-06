// $Id$

//=============================================================================
/**
 *  @file    BS2Gateway.cpp
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

#include "BS2Gateway.h"
#include "BS2DeclAtoms.h"
#include "BS2SECSReceiver.h"
#include "BS2AnalyzeReceiver.h"
#include "BS2AnalyzeSender.h"
#include "BS2Serial.h"
#include "BS2Socket.h"
#include "BS2Driver.h"
#include "BS2TransactionManager.h"
#include "BS2MessageInfo.h"
#include "BS2ErrorMessage.h"
#include "BS2Peer.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Gateway::BS2Gateway() : BS2Device()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Gateway::BS2Gateway");
}

//-----------------------------------------------------------------------------
BS2Gateway::~BS2Gateway()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Gateway::~BS2Gateway");
}

//-----------------------------------------------------------------------------
// Create new receiver and sender.
//-----------------------------------------------------------------------------
int BS2Gateway::initialize(DeviceParameter * parm)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Gateway::initialize");

    int result;
    ACE_TCHAR name[64];
    SocketParameter peerParam;

    result = this->initxml(parm);
    if (result < 0)
    {
        return result;
    }

    m_sender = new BS2AnalyzeSender(this);
    m_devid = parm->m_deviceId;
    m_action = (parm->m_slave == 0) ? EQUIPMENT : HOST;
    m_trmgr->sourceId(parm->m_sourceId);
    if (parm->m_dtype == DRIVER_SOCKET)
    {
        BS2AnalyzeReceiver * hsms = new BS2AnalyzeReceiver(this);
        m_receiver = hsms;

        BS2Peer * socket = new BS2Peer();
        m_driver = socket;

        BS2Peer * peer_socket = new BS2Peer();
        m_peer = peer_socket;

        socket->peer(peer_socket);
        peer_socket->peer(socket);

        SocketParameter * paramPtr = (SocketParameter *)parm;
        _tcscpy(peerParam.m_xmlname, paramPtr->m_xmlname);
        peerParam.m_deviceId = paramPtr->m_deviceId;
        peerParam.m_sourceId = paramPtr->m_sourceId;
        peerParam.m_dtype = paramPtr->m_dtype;
        peerParam.m_slave = (paramPtr->m_slave) ? 0 : 1;  // 0=equipment, 1=slave
        peerParam.m_t3timeout = paramPtr->m_t3timeout;
        peerParam.m_hbtimeout = paramPtr->m_hbtimeout;
        peerParam.m_mode = (paramPtr->m_mode) ? 0 : 1; // 0=active, 1=passive
        peerParam.m_port = paramPtr->m_peer_port;      // port number
        _tcscpy(peerParam.m_hostname, paramPtr->m_hostname);
        peerParam.m_t5timeout = paramPtr->m_t5timeout;
        peerParam.m_t6timeout = paramPtr->m_t6timeout;
        peerParam.m_t7timeout = paramPtr->m_t7timeout;
        peerParam.m_t8timeout = paramPtr->m_t8timeout;
        peerParam.m_peer_port = paramPtr->m_port;      // peer port number
        peerParam.m_dump = 0;                          // dump off

        ACE_OS::strcpy(name, "HSMS");
        m_sender->T6(((SocketParameter *)parm)->m_t6timeout);
    }
    else
    {
        BS2SECSReceiver * secs1 = new BS2SECSReceiver(this);
        m_receiver = secs1;
        BS2Serial * serial = new BS2Serial();
        m_driver = serial;
        ACE_OS::strcpy(name, "SECS-I");
    }

    // initial control tables
    if (m_driver->initialize(parm, this) >= 0)
    {
        result = m_driver->open();
        if (result >= 0)
        {
            if (m_peer->initialize(&peerParam, this) < 0)
            {
                ;
            }
            if (m_sender->open() >= 0)
            {
                m_sender->T3(parm->m_t3timeout);
                m_sender->setLinktestTime(parm->m_hbtimeout);
                if (m_receiver->open() >= 0)
                {
                    return BEE_SUCCESS;
                }
                else
                {
                    TRACE_ERROR((_TX("(%t) receiver is not opend %s %d\n"),
                                 name, parm->m_deviceId));
                }
            }
            else
            {
                TRACE_ERROR((_TX("(%t) sender is not opend %s %d\n"),
                             name, parm->m_deviceId));
            }
        }
        else
        {
            TRACE_ERROR((_TX("(%t) device module is not opened %s %d\n"),
                         name, parm->m_deviceId));
        }
    }
    else
    {
        TRACE_ERROR((_TX("(%t) device module is not initalized %s %d\n"),
                     name, parm->m_deviceId));
    }

    // fail open module.
    delete m_driver;
    m_driver = NULL;
    delete m_sender;
    m_sender = NULL;
    delete m_receiver;
    m_receiver = NULL;
    return BEE_ERROR;
}


//-----------------------------------------------------------------------------
// Disconnected communication.
//-----------------------------------------------------------------------------
int BS2Gateway::disconnected()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Gateway::disconnected");

    if (((BS2Peer *)m_driver)->getStatus() != HSMS_NOT_CONNECTED)
    {
        ((BS2Peer *)m_driver)->disconnect_endpoint();
    }
    else if (((BS2Peer *)m_peer)->getStatus() != HSMS_NOT_CONNECTED)
    {
        ((BS2Peer *)m_peer)->disconnect_endpoint();
    }

    return true;
}

//-----------------------------------------------------------------------------
// Open peer driver. (for Gateway driver)
//-----------------------------------------------------------------------------
int  BS2Gateway::peerOpen()
{
    int result = m_peer->open();
    return result;
}

