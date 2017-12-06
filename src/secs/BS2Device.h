// $Id: BS2Device.h,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Device.h
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

#ifndef BS2DEVICE_H
#define BS2DEVICE_H

#include "beesecs.h"
#include "BS2Message.h"

//-----------------------------------------------------------------------------
// Socket Driver initialize parameter
//-----------------------------------------------------------------------------
enum { DRIVER_SERIAL = 0, DRIVER_SOCKET = 1 };
struct DeviceParameter
{
    BCHAR m_xmlname[256];       // xml file which define secs message format
    int m_deviceId;
    int m_sourceId;
    int m_dtype;
    int m_slave;               // 0=equipment, 1=slave
    int m_dump;
    long m_t3timeout;
    long m_hbtimeout;
//
    DeviceParameter(int type = DRIVER_SOCKET)
    : m_deviceId(0), m_sourceId(1), m_dtype(type), m_slave(0),
      m_dump(0), m_t3timeout(45000), m_hbtimeout(0) 
	{
            _tcscpy(m_xmlname, _TX("secs.xml"));
    }
};

//
// Serial Driver initialize parameter
//
struct CommParameter: DeviceParameter
{
    BCHAR m_port[16];          // comm port name
    int  m_baudrate;
    BCHAR m_parity[8];
    int  m_databit;
    int  m_stopbit;
    int  m_read_timeout;
    int  m_retry;
    long m_t2timeout;
    long m_t4timeout;
//
    CommParameter(): DeviceParameter(DRIVER_SERIAL),
                     m_baudrate(9600), m_databit(0), m_stopbit(0),
                     m_read_timeout(15), m_retry(3), m_t2timeout(10000),
                     m_t4timeout(45000)
         { m_port[0] = '\0'; _tcscpy(m_parity, _TX("none")); }
};

//
// Socket Driver initialize parameter
//
struct SocketParameter: DeviceParameter
{
    int m_mode;                         // 0=active, 1=passive
    int m_port;                         // port number
    BCHAR m_hostname[256];              // network address
    long m_t5timeout;
    long m_t6timeout;
    long m_t7timeout;
    long m_t8timeout;
    int   m_peer_port;                  // peer port number
    BCHAR m_peername[256];              // network address
//
    SocketParameter(): DeviceParameter(DRIVER_SOCKET),
                       m_port(HSMS_PORT_NUMBER),
                       m_t5timeout(3000), m_t6timeout(5000),
                       m_t7timeout(10000), m_t8timeout(5000),
                       m_peer_port(0)
    {
        _tcscpy(m_hostname, _TX("localhost"));
		//_tcscpy(m_hostname, _TX("192.168.0.23"));
        m_peername[0] = '\0';
    }
};

// Forward declaration
class BS2Driver;
class BS2Sender;
class BS2Message;
class BS2Receiver;
class BS2TransactionManager;
class BS2MessageInfo;

//-----------------------------------------------------------------------------
//
// MultiBlock control class
//
//-----------------------------------------------------------------------------
class BS2MultiBlock
{
public:
    BS2MultiBlock(BS2Message * msg) : m_orgmsg(msg), m_inqtid(0) {}
    ~BS2MultiBlock() {}

    BS2Message * message() { return m_orgmsg; }
    void inqNum(int num)   { m_inquire = num;  }
    int  inqNum()          { return m_inquire;  }
    int  trans()     { return m_orgmsg->transNum(); }
    void tid(int id) { m_inqtid = id; }

private:
    BS2Message * m_orgmsg;
    int          m_inquire;   // inquiry stream/function
    int          m_inqtid;    // tid of inquiry message
};


//-----------------------------------------------------------------------------
//
// SECS Device Module
//
//-----------------------------------------------------------------------------
class BEE_Export BS2Device: public MT_Task
{
public:
    enum { EQUIPMENT = 0, HOST = 1 };

public:
    BS2Device();
    ~BS2Device();

	int	initialize(DeviceParameter * parm, BS2Sender* local_sender, BS2Receiver*	local_receiver);
    
	virtual int open(void * = 0);
    virtual int close(int = 0);

	virtual int parse(void* data, int size);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
    virtual int svc(void);
    virtual int connected()    { return BEE_SUCCESS; }
    virtual int disconnected();

    int initxml(DeviceParameter * parm);
    int send(BS2Message * msg);
    int send(BS2Message * msg, void * mngr);   // Check multi-block
    int send_s(BS2Message * msg, void * mngr); // Non-check multi-block
    //int receive(BS2MessageInfo& rinfo);
	int receive(BS2MessageInfo* ret, bool needtimeout);
    int receiveWithLimit(BS2MessageInfo* rinfo, ACE_Time_Value *tv = 0);
	int sense(BS2MessageInfo* evtinfo);
    int restart(void * = NULL);
    int stop_receive();
    int cancelReceive();
    int putCommand(BCHAR * cmd);

    BS2Driver * getDriver() const { return m_driver; }
    BS2Sender * getSender() const { return m_sender; }
    BS2Receiver * getReceiver() const { return m_receiver; }
    BS2TransactionManager * getTransactionManager() const { return m_trmgr; }
    int  getDeviceId() const { return m_devid; }
    int  setDeviceId(int id);
    void setName(const BCHAR * name) {
            _tcsncpy(m_name, name, sizeof(m_name) - 1);
            m_name[sizeof(m_name) - 1] = '\0';
        }
    const BCHAR * getName()   { return m_name; }
    bool  isHost() const      { return (m_action == HOST); }
    bool  isEquipment() const { return (! isHost()); }
    void  hexDump(bool tf = true);

	volatile int m_bs2device_play;
	volatile int m_bs2device_loop;

protected:
    BS2Message * addInquiry(BS2MultiBlock * multinq);
    BS2Message * removeInquiry(BS2Message * grntmsg);

    int  m_devid;
    int  m_action;
    BCHAR m_name[48];
    BS2TransactionManager * m_trmgr;
    MUTEX_Bool  m_changeLog;
    BS2Driver * m_driver;
    BS2Driver * m_peer;                 // for Gateway
    BS2Sender * m_sender;
    BS2Receiver * m_receiver;
    void * m_obj;

    ACE_Thread_Mutex m_lock;
    list<BS2MultiBlock *, allocator<BS2MultiBlock *> > m_multi;

};

#endif
