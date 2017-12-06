// $Id: BS2SECSReceiver.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2SECSReceiver.cpp
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

#include "ace/OS.h"
#include "BS2Device.h"
#include "BS2SECSReceiver.h"
#include "BS2Sender.h"
#include "BS2Message.h"
#include "BS2Stream.h"
#include "BS2BlockHeader.h"
#include "BS2Driver.h"
#include "BS2MessageInfo.h"
#include "BS2ErrorMessage.h"
#include "BS2MessageDictionary.h"

//------------------------------------------------------------------------------
// parse receive data.
//------------------------------------------------------------------------------
int BS2SECSReceiver::parse(BCHAR * data, int size)
{
    TRACE_FUNCTION(TRL_LOW, "BS2SECSReceiver::parse");

    BS2TransactionManager * trmgr = m_device->getTransactionManager();
    BS2BlockHeader * curHeader = (BS2BlockHeader *)data;
    BS2BlockHeader beforeHeader;
    BS2OStream * ostmbuf;
    BS2TransactionInfo * trinfo;
    BS2MessageInfo * evtinfo;
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
    if (trinfo != NULL)                 // Expected block
    {
        ostmbuf = trinfo->buffer();
        if (trinfo->type() == TRANSACTION_SEND_PRIMARY ||
            trinfo->type() == TRANSACTION_RECV_SECONDARY)
        {   // Secondary message
            ostmbuf->blockHeader(beforeHeader);
            if (beforeHeader.isExpect(curHeader) ||
                (beforeHeader.getStreamFunctionNum() == curHeader->getStreamFunctionNum()))
            {
                if (curHeader->getBlockNum() <= 1)      // First block
                {
                    // Cancel reply timer
                    this->cancelTimer(trinfo->timer_id());

                    // First block of Secondary message
                    ostmbuf->reset();
                    ostmbuf->write(data, size);
                    // Change transaction type.
                    trinfo->type(TRANSACTION_RECV_SECONDARY);
                }
                else
                {   // N'th block
                    // Cancel inter-block timer
                    this->cancelTimer(trinfo->timer_id());

                    // Add block to secondary message
                    ostmbuf->write(data + sizeof(BS2BlockHeader),
                                   size - sizeof(BS2BlockHeader));
                }

                if (curHeader->end())   // Last block ?
                {
                    msg = BS2MessageDictionary::instance()->make(ostmbuf);
                    if (msg == NULL)
                    {   // Illegl message
                        if (m_device->isEquipment())
                        {
                            s9f7 = new BS2S9F7Message(*curHeader);
                            sender->send(s9f7);
                            delete s9f7;
                        }
                        // Release transaction info.
                        evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                        if (evtinfo == NULL)
                        {
                            TRACE_ERROR((_TX("Transaction-info locked(1). \n")));
                            TRACE_ERROR((_TX("Receive illegal data.\n")));
                            return BEE_ERROR;
                        }
                        
						//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
						this->putEventInfo(evtinfo);   // signal to wait in receive method.

                        TRACE_ERROR((_TX("Receive illegal data.\n")));
                        return BEE_ERROR;
                    }
                    else
                    {
                        if (msg->noise())       // Found un-supported message
                        {
                            if (m_device->isEquipment())
                            {
                                s9f7 = new BS2S9F7Message(*curHeader);
                                sender->send(s9f7);
                                delete s9f7;
                            }
                            delete msg;        // Remove Noise

                            evtinfo = trmgr->info(trinfo, BS2RET_ILLEGAL_MESSAGE, NULL);
                            if (evtinfo == NULL)
                            {
                                TRACE_ERROR((_TX("Tansaction-info locked(2).\n")));
                                TRACE_ERROR((_TX("Receive noise message data.\n")));
                                return BEE_ERROR;
                            }
                            
							//this->notifyEventInfo(evtinfo);   // signal to wait in receive method.
							this->putEventInfo(evtinfo);   // signal to wait in receive method.

                            TRACE_ERROR((_TX("Receive noise message data.\n")));
                            return BEE_ERROR;
                        }

                        // Get MsgControl and Removed trinfo
                        evtinfo = trmgr->info(trinfo, BS2RET_NORMAL, msg);
                        if (evtinfo == NULL)
                        {
                            TRACE_ERROR((_TX("Tansaction-info locked(3).\n")));
                            return BEE_ERROR;
                        }
                        
						//this->notifyEventInfo(evtinfo);   // signal to wiat in receive method.
						this->putEventInfo(evtinfo);   // signal to wait in receive method.

                    }
                    // Complete secondary
                    // trinfo->ok_func()->call();
                }
                else
                {   // Cancel inter-block timer
                    this->addTimer(m_T4, trinfo);

                    // Set expected block
                    // trmgr->insert(TRANSACTION_RECV_PRIMARY, stmbuf, NULL, NULL);
                    // * Set above
                }
            }
            else
            {
                TRACE_ERROR((_TX("unexpected block \n")));
                beforeHeader.dump();
                return BEE_ERROR;
            }
        }
        else
        {   // Primary message
            // Cancel inter-block timer
            this->cancelTimer(trinfo->timer_id());

            // First block of primary message
            ostmbuf->write(data + sizeof(BS2BlockHeader),
                           size - sizeof(BS2BlockHeader));
            if (curHeader->end())           // Last block ?
            {
                msg = BS2MessageDictionary::instance()->make(ostmbuf);
                if (msg == NULL)
                {   // un-supported message
                    if (m_device->isEquipment())
                    {
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }
                    if (trmgr->remove(trinfo) < 0)
                    {
                        TRACE_ERROR((_TX("Transaction-remove locked(3). \n")));
                    }
                    TRACE_ERROR((_TX("Receive illegal data")));
                    return BEE_ERROR;
                }
                else
                {   // Complete primary message
                    if (msg->noise())
                    {
                        if (m_device->isEquipment())
                        {
                            s9f7 = new BS2S9F7Message(*curHeader);
                            sender->send(s9f7);
                            delete s9f7;
                        }
                        if (trmgr->remove(trinfo) < 0)
                        {
                            TRACE_ERROR((_TX("Transaction-remove locked(4). \n")));
                        }
                        TRACE_ERROR((_TX("Illegal SECS2 message data.\n")));
                        return BEE_ERROR;
                    }
                    else
                    {
                        evtinfo = trinfo->eventHeader();
                        evtinfo->setResult(BS2RET_NORMAL);
                        evtinfo->setMessage(msg);
                        
						//this->notifyEventInfo(evtinfo);       // signal to wiat in receive method.
						this->putEventInfo(evtinfo);   // signal to wait in receive method.

                        if (curHeader->wait())      // W bit = 1 ?
                        {   // Set system bytes for reply
                            // trmgr->insert(TRANSACTION_RECV_PRIMARY, istmbuf, NULL, NULL);
                        }
                        else
                        {
                            if (trmgr->remove(trinfo) < 0)
                            {
                                TRACE_ERROR((_TX("Transaction-remove locked(5). \n")));
                            }
                        }
                    }
                }
            }
            else
            {   // Set block
                this->addTimer(m_T4, trinfo);

                // Set expected block
                // trmgr->insert(TRANSACTION_RECV_PRIMARY, istmbuf, NULL);
                // * Set above
            }
        }
    }
    else
    {   // Not multi-block message
        if (curHeader->isPrimary() && curHeader->getBlockNum() <= 1)
        {   // primary message & first block
            ostmbuf = new BS2OStream;
            ostmbuf->write(data, size);
            if (curHeader->end())    // end message
            {
                msg = BS2MessageDictionary::instance()->make(ostmbuf);
                if (msg == NULL)
                {   // illegal data
                    if (m_device->isEquipment())
                    {
                        s9f7 = new BS2S9F7Message(*curHeader);
                        sender->send(s9f7);
                        delete s9f7;
                    }
                    TRACE_ERROR((_TX("Don't make SECS2 message object.\n")));
                    return BEE_ERROR;
                }
                else
                {   // As complete primary, queued message
                    if (msg->noise())
                    {
                        TRACE_ERROR((_TX("Illegal SECS2 message data.\n")));
                        if (m_device->isEquipment())
                        {
                            s9f7 = new BS2S9F7Message(*curHeader);
                            sender->send(s9f7);
                            delete s9f7;
                        }
                        delete msg;             // Remove Noise
                        return BEE_ERROR;
                    }
                    else
                    {
                        evtinfo = new BS2MessageInfo;
                        if (curHeader->wait())      // W bit on
                        {
                            trinfo = trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, evtinfo);
                            if (trinfo == NULL)
                            {   // Panic !
                                delete evtinfo;
                                delete ostmbuf;
                                TRACE_ERROR((_TX("Can not create transaction info.\n")));
                                return BEE_ERROR;
                            }
                            evtinfo = trinfo->eventHeader();
                        }
                        evtinfo->setResult(BS2RET_NORMAL);
                        evtinfo->setMessage(msg);
                        
						//this->notifyEventInfo(evtinfo);       // signal to wiat in receive method.
						this->putEventInfo(evtinfo);   // signal to wait in receive method.

                        if (curHeader->wait())      // W bit on
                        {   // Set system bytes for reply
                            // trmgr->insert(TRANSACTION_RECV_PRIMARY, istmbuf, NULL);
                            // * Set above
                        }
                    }
                }
            }
            else
            {   // Set inter-block timer
                this->addTimer(m_T4, trinfo);

                // Set expected block
                evtinfo = new BS2MessageInfo;
                trinfo = trmgr->insert(TRANSACTION_RECV_PRIMARY, ostmbuf, evtinfo);
                if (trinfo == NULL)
                {   // Panic !
                    delete evtinfo;
                    delete ostmbuf;
                    TRACE_ERROR((_TX("Can not create transaction info.\n")));
                    return BEE_ERROR;
                }
            }
        }
        else
        {
            TRACE_ERROR((_TX("Don't found primary message.\n")));
            trmgr->dump();
            return BEE_ERROR;            // Block error
        }
    }
    return BEE_SUCCESS;

}


