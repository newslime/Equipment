// $Id: BS2HSMSReceiver.cpp,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2HSMSReceiver.cpp
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

#include "BS2HSMSReceiver.h"
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
// parse receive data.
//-----------------------------------------------------------------------------
int BS2HSMSReceiver::parse(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2HSMSReceiver::parse");

    BS2TransactionManager * trmgr = m_device->getTransactionManager();
    BS2BlockHeader * curHeader = (BS2BlockHeader *)data;
    BS2BlockHeader beforeHeader;
    BS2OStream * ostmbuf;
    BS2TransactionInfo * trinfo;
    BS2MessageInfo * evtinfo;
    BS2MessageInfo msgcont;
    BS2Message * msg;
    BS2S9F7Message * s9f7;
    BS2Sender * sender = m_device->getSender();

    if (curHeader->getStreamNum() == 9)
    {
        this->parseStreamNine(data, size);   // case S9F9, timer cancel
        return BEE_ERROR;
    }

    if (curHeader->isPrimary())
    {
        trinfo = trmgr->buffer(curHeader, TRANSACTION_RECV_PRIMARY);
        while ((unsigned long)trinfo == 0xFFFFFFFF)
        {
            TRACE_ERROR((_TX("Tansaction-buffer locked(1).\n")));
            trinfo = trmgr->buffer(curHeader, TRANSACTION_RECV_PRIMARY);
        }
    }
    else
    {
        trinfo = trmgr->buffer(curHeader);
        while ((unsigned long)trinfo == 0xFFFFFFFF)
        {
            TRACE_ERROR((_TX("Tansaction-buffer locked(2).\n")));
            trinfo = trmgr->buffer(curHeader);
        }
    }
    if (trinfo != NULL)                 // Expected block ?
    {
        ostmbuf = trinfo->buffer();
        if (trinfo->type() == TRANSACTION_SEND_PRIMARY)
        {   // Secondary message
            ostmbuf->blockHeader(beforeHeader);
            if (beforeHeader.isExpect(curHeader) || (curHeader->getFunctionNum() == 0))
            {
                // Cancel reply timer
                this->cancelTimer(trinfo->timer_id());

                // First block of secondary message
                ostmbuf->reset();
                ostmbuf->write(data, size);

                msg = BS2MessageDictionary::instance()->make(ostmbuf);
                if (msg == NULL)
                {   // Unsupportted message
                    if (m_device->isEquipment())
                    {
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }

                    evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                    if (evtinfo == NULL)
                    {
                        TRACE_ERROR((_TX("Tansaction-info locked(3).\n")));
                        TRACE_ERROR((_TX("Receive illegal data.\n")));
                        trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }
                    
					//this->notifyEventInfo(evtinfo);   // signal to wait in receiving method.
					this->putEventInfo(evtinfo);   // signal to wait in receive method.

                    TRACE_ERROR((_TX("Receive illegal data.\n")));
                    trmgr->remove(trinfo);
                    return BEE_ERROR;
                }
                else
                {
                    if (msg->noise())
                    {
                        TRACE_ERROR((_TX("Receive noise message data.\n")));
                        if (m_device->isEquipment())
                        {
                            s9f7 = new BS2S9F7Message(*curHeader);
                            sender->send(s9f7);
                            delete s9f7;
                        }
                        delete msg;    // Remove Noise

                        evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                        if (evtinfo == NULL)
                        {
                            TRACE_ERROR((_TX("Tansaction-info locked(4).\n")));
                            trmgr->remove(trinfo);
                            return BEE_ERROR;
                        }
                        
						//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
						this->putEventInfo(evtinfo);   // signal to wait in receive method.
                        
						trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }

                    // Get MsgControl and Removed trinfo
                    evtinfo = trmgr->info(trinfo, BS2RET_NORMAL, msg);
                    if (evtinfo == NULL)
                    {
                        TRACE_ERROR((_TX("Tansaction-info locked(5).\n")));
                        trmgr->remove(trinfo);
                        return BEE_ERROR;
                    }
                    
					//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
					this->putEventInfo(evtinfo);   // signal to wait in receive method.
                }
            }
            else
            {   // unexpeted block
                TRACE_ERROR((_TX("HSMS does not support multi block.\n%s\n"),
                             _TX("Mismatched transaction number.")));
                TRACE_ERROR((_TX("Probably, received primary message is in losted trasaction number.\n")));
                trinfo->dump();
                if (trmgr->remove(trinfo) < 0)
                {
                    TRACE_ERROR((_TX("Transaction-remove locked(6). \n")));
                }
                return BEE_ERROR;
            }
        }
        else
        {   // Found multi-block of primary
            TRACE_ERROR((_TX("HSMS: Not multi block.\n")));
            beforeHeader.dump();
            return BEE_ERROR;
        }
    }
    else
    {   // Not multi-block message
        if (curHeader->isPrimary() && curHeader->getBlockNum() <= 1)
        {   // First block of primary
            ostmbuf = new BS2OStream;
            ostmbuf->write(data, size);

            msg = BS2MessageDictionary::instance()->make(ostmbuf);
            if (msg == NULL)
            {   // Un-supportted message
                if (m_device->isEquipment())
                {
                    s9f7 = new BS2S9F7Message(*curHeader);
                    sender->send(s9f7);
                    delete s9f7;
                }
                TRACE_ERROR((_TX("Don't make SECS-II message object.\n")));
                return BEE_ERROR;
            }
            else
            {   // Complete primary, message queued
                if (msg->noise())
                {
                    if (m_device->isEquipment())
                    {
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }
                    TRACE_ERROR((_TX("Illegal SECS-II message data.\n")));
                    return BEE_ERROR;
                }
                else
                {
                    evtinfo = new BS2MessageInfo;
                    if (curHeader->wait())               // W bit on
                    {   // Save system bytes for reply
                        trinfo = trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, evtinfo);
                        if (trinfo == NULL)
                        {
                            TRACE_ERROR((_TX("Can not create transaction info.\n")));
                            delete evtinfo;
                            delete ostmbuf;
                            return BEE_ERROR;            // Panic !
                        }
                        evtinfo = trinfo->eventHeader();
                    }
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
        }
        else
        {
            TRACE_ERROR((_TX("Don't found primary message.\n")));
            trmgr->dump();
            return BEE_ERROR;            // Discard block
        }
    }
    return BEE_SUCCESS;

}

