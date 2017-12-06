// $Id$

//=============================================================================
/**
 *  @file    BS2AnalyzeReceiver.cpp
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

#include "BS2AnalyzeReceiver.h"
#include "BS2Sender.h"
#include "BS2Message.h"
#include "BS2Stream.h"
#include "BS2BlockHeader.h"
#include "BS2Device.h"
#include "BS2Driver.h"
#include "BS2MessageInfo.h"
#include "BS2ErrorMessage.h"
#include "BS2MessageDictionary.h"

//-----------------------------------------------------------------------------
// Dispatch receive data.
//-----------------------------------------------------------------------------
int BS2AnalyzeReceiver::dispatch(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2AnalyzeReceiver::dispatch");
    BS2BlockHeader * bheader = (BS2BlockHeader *)(data);
    int stnum = bheader->getStreamNum();
    int fcnum = bheader->getFunctionNum();

    // Check stream number.
    if (! BS2MessageDictionary::instance()->existStreamNum(stnum))
    {
        if (m_device->isEquipment())
        {
            BS2S9F3Message * s9f3 = new BS2S9F3Message(*bheader);
            m_device->getSender()->send(s9f3);
        }
        return BEE_SUCCESS;       // not send NAK
    }

    // Check function number.
    if (BS2MessageDictionary::instance()->search(SFCODE(stnum, fcnum)) == NULL)
    {
        if (m_device->isEquipment())
        {
            BS2S9F5Message * s9f5 = new BS2S9F5Message(*bheader);
            m_device->getSender()->send(s9f5);
        }
        return BEE_SUCCESS;       // not send NAK
    }

    ACE_Message_Block * mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(size + 16), BEE_ERROR);
    memcpy(mb->rd_ptr(), data, size);
    mb->length(size);
    this->put(mb);

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
// parse receive data.
//-----------------------------------------------------------------------------
int BS2AnalyzeReceiver::parse(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2AnalyzeReceiver::parse");

    BS2TransactionManager * trmgr = m_device->getTransactionManager();
    BS2BlockHeader * curHeader = (BS2BlockHeader *)data;
    BS2BlockHeader beforeHeader;
    BS2OStream * ostmbuf;
    BS2MessageInfo * evtinfo;
    BS2MessageInfo msgcont;
    BS2Message * msg;
    BS2Sender * sender = m_device->getSender();

    ostmbuf = new BS2OStream;
    ostmbuf->write(data, size);

    msg = BS2MessageDictionary::instance()->make(ostmbuf);
    if (msg == NULL)
    {   // Un-supportted message
        TRACE_ERROR((_TX("Don't Make SECS-II Message Object.\n")));
        return BEE_ERROR;
    }
    else
    {   // Complete primary, message queued
        if (msg->noise())
        {
            TRACE_ERROR((_TX("Illegal SECS-II message data.\n")));
            return BEE_ERROR;
        }
        else
        {
            evtinfo = new BS2MessageInfo;
            delete ostmbuf;
            evtinfo->setDelete();
            evtinfo->setResult(BS2RET_NORMAL);
            evtinfo->setMessage(msg);
            
			//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
			this->putEventInfo(evtinfo);   // signal to wait in receive method.

            if (curHeader->wait())      // W bit on
            {   // Save system bytes for reply
                // trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, NULL);
                // * Saved above
            }
        }
    }
    return BEE_SUCCESS;

}

