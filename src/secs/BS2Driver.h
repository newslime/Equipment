// $Id: BS2Driver.h,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Driver.h
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

#ifndef BS2DRIVER_H
#define BS2DRIVER_H

#include "beesecs.h"

struct DeviceParameter;
class BS2Device;
class BS2Sender;
class BS2Receiver;
class BS2TransactionInfo;
class BS2OStream;

typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> SEMIStatus;

//-----------------------------------------------------------------------------
// SECS Driver
//-----------------------------------------------------------------------------
class BEE_Export BS2Driver
{

friend class BS2Receiver;
friend class BS2SECSReceiver;
friend class BS2HSMSReceiver;

// constructor/destoructor
public:
    BS2Driver();
    virtual ~BS2Driver();
//
    virtual void dump() const = 0;
    virtual int  initialize(DeviceParameter * config, BS2Device * device) = 0;
    virtual int  open() = 0;
    virtual int  recv(BCHAR * buf, int bufSize) = 0;
    virtual int  send(BS2TransactionInfo * bufHandle) = 0;
    virtual int  close() = 0;
    virtual int  recv_thread() = 0;
    virtual bool canSend() const = 0;
    virtual BS2OStream * makeLinktestMessage() = 0;
    virtual int  enable() = 0;
    virtual int  disable() = 0;
    virtual int  disconnect_endpoint() = 0;

    void deviceId(int devid) { m_deviceId = devid; }
    void sourceId(int srcid) { m_sourceId = srcid; }
    bool isOpened() const    { return m_open; }
    bool isEnable() const;
    bool isHost() const { return m_slave; }
    bool isEquipment() const { return (! isHost()); }
    void hexDump(bool cond) { m_hexdump = cond; }

	volatile int m_bs2driver_loop;

protected:
    void   sendbuf(BCHAR * bufptr, int size) {
                m_sendbuf = bufptr;
                m_sendbufSize = size;
            }
    BCHAR * sendbuf() const { return m_sendbuf; }
    void   recvbuf(BCHAR * bufptr, int size) {
                m_recvbuf = bufptr;
                m_recvbufSize = size;
            }
    BCHAR * recvbuf() const     { return m_recvbuf; }
    int    recvbufSize() const { return m_recvbufSize; }
    int    recvSize() const    { return m_recvSize; }
    int    send(BCHAR * data, int dataSize);

// variables
protected:
    int    m_deviceId;
    int    m_sourceId;
    bool   m_slave;
    bool   m_open;
    BCHAR * m_sendbuf;
    int    m_sendbufSize;
    int    m_sendSize;
    BCHAR * m_recvbuf;
    int    m_recvbufSize;
    int    m_recvSize;
    BS2Device * m_device;
    MUTEX_Int  m_disable;
    bool  m_hexdump;
};

#endif
