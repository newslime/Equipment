// $Id: BS2Message.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Message.cpp
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

#include "BS2Message.h"
#include "BS2ListItem.h"
#include "BS2List.h"
#include "BS2Stream.h"
#include "BS2MessageDictionary.h"
#include "BS2ItemDictionary.h"
#include "BS2BlockHeader.h"
#include "BS2value.h"
#include "BS2Interpreter.h"
#include "BS2DeclAtoms.h"
#include "BS2Traverser.h"

typedef stack<BS2Items::iterator> IterStack;

typedef struct MessageTraverseInfo
{
    BS2ItemList::iterator m_topIter;
    BS2Items::iterator    m_curIter;
    IterStack       m_listIter;
    void *          m_clientData;
    ClientProcedure m_proc;
} MessageTraverseInfo;


//-----------------------------------------------------------------------------
// set
//-----------------------------------------------------------------------------
void BS2Message::set(const BS2BlockHeader * header)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::set");

    m_wbit = header->wait() ? true : false;
    m_deviceNum = header->getSessionId();   // incluse reverse bit
    m_streamNum = header->getStreamNum();
    m_functionNum = header->getFunctionNum();
    m_blockNum = header->getBlockNum();
    UINT srcnum = header->getSourceNum();
    UINT trnum  = header->getTransactionNum();
    m_transactionNum = (srcnum << 16) | trnum;
}

//-----------------------------------------------------------------------------
// get
//-----------------------------------------------------------------------------
int BS2Message::get(BS2BlockHeader * header) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::get");

    header->resetToHostBit();
    header->wait(m_wbit);
    header->setSessionId(m_deviceNum);
    header->setStreamNum(m_streamNum);
    header->setFunctionNum(m_functionNum);
    header->setBlockNum(m_blockNum);
    header->setSourceNum((m_transactionNum >> 16) & 0xFFFF);
    header->setTransactionNum(m_transactionNum & 0xFFFF);
    return sizeof(BS2BlockHeader);
}

//-----------------------------------------------------------------------------
// Check expected message or not
//-----------------------------------------------------------------------------
bool BS2Message::isExpect(const BS2BlockHeader& header) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::get");

    if ((m_deviceNum == (int)header.getSessionId()) &&
        (m_streamNum == (int)header.getStreamNum()) &&
        (m_functionNum == (int)header.getFunctionNum()) &&
        (m_transactionNum == (UINT)header.getTransactionNum()))
        return true;
    else
        return false;
}



///////////////////////////////////////////////////////////////////////////////
//
// B S 2 M e s s a g e
//
//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2Message::BS2Message()
        : m_msgtype(NULL), m_dataSize(0), m_itemCount(0), m_complete(0),
          m_spooled(false),
          m_wbit(false), m_deviceNum(1), m_streamNum(0), m_functionNum(0),
          m_blockNum(0), m_transactionNum(0)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Message::BS2Message");
}

//-----------------------------------------------------------------------------
BS2Message::BS2Message(int device, bool w, int stnum, int funcnum,
                       int blocknum, UINT trans)
        : m_msgtype(NULL), m_dataSize(0), m_itemCount(0), m_complete(0),
          m_spooled(false),
          m_wbit(w), m_deviceNum(device), m_streamNum(stnum),
          m_functionNum(funcnum), m_blockNum(blocknum),
          m_transactionNum(trans)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Message::BS2Message");

    m_msgtype = BS2MessageDictionary::instance()->search(SFCODE(stnum, funcnum));

}

//-----------------------------------------------------------------------------
BS2Message::BS2Message(UINT sf) : m_dataSize(0), m_itemCount(0), m_complete(0),
                                  m_spooled(false)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Message::BS2Message");

    m_msgtype = BS2MessageDictionary::instance()->search(sf);
    
	if (m_msgtype != NULL)
    {
        m_wbit = m_msgtype->wait();
        m_streamNum = STREAMCODE(m_msgtype->sfnum());
        m_functionNum = FUNCCODE(m_msgtype->sfnum());
    }
    else
    {
        m_wbit = false;
        m_streamNum = 0;
        m_functionNum = 0;
    }
    m_deviceNum = 1;   // incluse reverse bit
    m_blockNum = 0;
    m_transactionNum = 0;
}

//-----------------------------------------------------------------------------
BS2Message::~BS2Message()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2Message::~BS2Message");

    releaseAll();
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
void BS2Message::copy(BS2ItemList const& rhs)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::copy");
    if (rhs.size() > 0)
    {
        BS2ItemList::const_iterator iter;
        for (iter = rhs.begin(); iter != rhs.end(); ++iter)
        {
            BS2Item * item = *iter;
            BS2Item * replica = item->replicate();
            m_items.push_back(replica);
        }
    }
}

//-----------------------------------------------------------------------------
// release
//-----------------------------------------------------------------------------
void BS2Message::releaseAll()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::releaseAll");
    if (m_items.size() > 0)
    {
        BS2ItemList::const_iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            BS2Item * item = *iter;
            delete item;
        }
    }
    m_items.clear();
}

//-----------------------------------------------------------------------------
bool BS2Message::noise() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::noise");
    return false;
}

//-----------------------------------------------------------------------------
// Factory object (static)
//-----------------------------------------------------------------------------
BS2Message * BS2Message::factory(UINT sf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::factory");
    BS2Message * result = BS2MessageDictionary::instance()->factory(sf);
    return result;
}

//-----------------------------------------------------------------------------
int BS2Message::factory_base(BS2IStream * buf)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::factory_base");
   
	//BS2Itemtypes& items = const_cast<BS2Itemtypes&>(m_msgtype->body());
    bool			retval;
	int				result;
	unsigned int	atomq;
	BS2BlockHeader	header;
	size_t			itemCount;
	BS2Atoms		atoms;
	BS2Atom*		atom;
	BS2ItemType*	itemtype;
	BS2Item*		item;
       
    if (buf == NULL)
        return -1;
    
	if (buf->size() < (int)sizeof(BS2BlockHeader))
        return -1;

    buf->header(header);
    this->set(&header);

	result = buf->makeAtoms(atoms);  // parse hex data
    
	if (result < 0)
        return result;

    itemCount	= 0;
    m_complete	= 1;
    
	if (atoms.size() > 0)
    {
        if (itemCount >= m_msgtype->body().size())
            throw runtime_error(_TX("BS2Message::factory_base()"));

        atomq = atoms.size();
        
		for (unsigned int i = 0; i < atomq; i++)
        {
            itemtype	= m_msgtype->body().at(itemCount);
            atom		= reinterpret_cast<BS2Atom *>(atoms[i]);

            if (itemtype->availFormat(atom->format() & ~ATOM_ARRAY))
            {
                item = itemtype->factory(atom);

				m_items.push_back(item); 
                
				if (item->isList() && itemtype->isList())
                {
                    BS2ListItem*	list_item = (BS2ListItem *)item;
                    BS2List*		list_atom = (BS2List *)atom; 

                    if (itemtype->isObject())
                    {
                        retval = parseAttrData((BS2Itemtypes *)itemtype,
                                               list_item,
                                               list_atom);
                    }
                    else
                    {
                        retval = listElements((BS2Itemtypes *)itemtype,
                                              list_item,
                                              list_atom);
                    }

                    if (!retval)
                    {
						//add by slime(1)
						delete list_atom;
						list_item->clearAtom();

						for(unsigned int j = i+1; j < atomq; j++)
						{
							atom = reinterpret_cast<BS2Atom *>(atoms[j]);

							if(atom->isList())
							{
								list_atom = (BS2List*)atom;
								EraseAtoms_Recursive(list_atom);
								delete list_atom;
							}
						}

                        return -1;
                    }

                    delete list_atom;
                    list_item->clearAtom();
                }
            }
            else
            {
                ACE_ERROR( (LM_ERROR, "BS2Message::factory_base : illegal item format (%s 0x%X).\n",
						   itemtype->itemName().c_str(), (atom->format() & ~ATOM_ARRAY)) );
                
				//add by slime(1)
				for(unsigned int j = i; j < atomq; j++)
				{
					atom = reinterpret_cast<BS2Atom *>(atoms[j]);

					if(atom->isList())
					{
						BS2List* list_atom = (BS2List*)atom;
						EraseAtoms_Recursive(list_atom);
						delete list_atom;
					}
					else if(atom->isArray())
					{
						//nothing
					}
					else
					{
						delete atom;
					}
				}
				
				return -1;
            }
            
			itemCount++;
            
			if (itemCount >= m_msgtype->body().size())
                break;    // if expect =< receive, then abort
        }
    }
    
	return 0;
}

//-----------------------------------------------------------------------------
// input list element
//-----------------------------------------------------------------------------
bool BS2Message::listElements(BS2Itemtypes* listStruct, 
							  BS2ListItem*	listItem,
                              BS2List*		listAtom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::listElements");
    	
    unsigned int atomq = listAtom->getList().size();
    
	if ((listStruct->size() > 1) && (listStruct->size() != atomq))
    {   // mismatch size of list
        if (atomq == 0)
		{
            return true;          // no item in list
		}
        else
		{
			EraseAtoms_Recursive(listAtom);
            return false;	
		}
    }

    bool			retval;
	bool			result;
    BS2Atom*		atom;
	BS2ItemType*	itemtype;
	BS2Item*		item;
	size_t			itemCount	= 0;
    BS2Atoms&		atoms		= listAtom->getList();
	    
	for (unsigned int i = 0; i < atomq; i++)
    {
        atom		= reinterpret_cast<BS2Atom *>(atoms[i]);
        itemtype	= listStruct->at(itemCount);
        
		if (itemtype == NULL)
        {
            TRACE_ERROR((_TX("Illegal message struct(%d).\n"), itemCount));
            atom->dump();
            continue;
        }
        
		result = itemtype->availFormat(atom->format() & ~ATOM_ARRAY);
        
		if (result)
        {
            item = itemtype->factory(atom);
            
			listItem->items().push_back(item);
			
			if (item->isList() && itemtype->isList())
            {
                BS2ListItem*	list_item = (BS2ListItem *)item;
                BS2List*		list_atom = (BS2List *)atom;

                if (itemtype->isObject())
                {
                    retval = parseAttrData((BS2Itemtypes *)itemtype,
                                           list_item,
                                           list_atom);
                }
                else
                {
                    retval = listElements((BS2Itemtypes *)itemtype,
                                          list_item,
                                          list_atom);
                }

                if (!retval)
                {
					//add by slime(2)
                    delete list_atom;
                    list_item->clearAtom();

					//remaining
					for(unsigned int j = i+1; j < atomq; j++)
					{
						atom = reinterpret_cast<BS2Atom *>(atoms[j]);

						if(atom->isList())
						{
							list_atom = (BS2List*)atom;
							EraseAtoms_Recursive(list_atom);
							delete list_atom;
						}
					}

                    return false;
                }
                
				delete list_atom;
                list_item->clearAtom();
            }
        }
        else
        {
            ACE_ERROR( (LM_ERROR, "BS2Message::listElements : illegal item format (%s 0x%X).\n",
					   itemtype->itemName().c_str(), (atom->format() & ~ATOM_ARRAY)) );
            
			m_complete = 0;
			
			//add by slime(2)
			for(unsigned int j = i; j < atomq; j++)
			{
				atom = reinterpret_cast<BS2Atom *>(atoms[j]);

				if(atom->isList())
				{
					BS2List* list_atom = (BS2List*)atom;
					EraseAtoms_Recursive(list_atom);
					delete list_atom;
				}
				else if(atom->isArray())
				{
					//nothing
				}
				else
				{
					delete atom;
				}
			}

            return false;          // Error-Log
        }
        
		itemCount++;
        
		if (itemCount == listStruct->size())
            itemCount = 0;
    }
    
	return true;
}

//-----------------------------------------------------------------------------
// input object attribute element (ATTRID + ATTRDATA)
//-----------------------------------------------------------------------------
bool BS2Message::parseAttrIDAndData(BS2Itemtypes * listStruct,
                                    BS2ListItem * listItem,
                                    BS2List * listAtom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::parseAttrIDAndData");
    BS2ItemType * itemtype;
    BS2Atom * atom;
    bool    result;

    unsigned int atomq = listAtom->getList().size();
    if (atomq == 0)
    {
        return true;          // no item in list
    }

    BS2Atoms& atoms = listAtom->getList();
    atom = reinterpret_cast<BS2Atom *>(atoms[0]);

    if (atom->isList())
    {   // Array of structure (ATTRID + ATTRDATA)
#if 0
        itemtype = listStruct->at(1);   // ATTRDATA's item type
        if (itemtype == NULL)
        {
            TRACE_ERROR((_TX("Illegal message struct(ATTRDATA).\n")));
            atom->dump();
            return false;
        }
        BS2Item * item = itemtype->factory(atom);
        listItem->items().push_back(item);
        BS2ListItem * list_item = (BS2ListItem *)item;
        BS2List * list_atom = (BS2List *)atom;
        result = parseAttrData(listStruct, list_item, list_atom);
#endif
        result = parseAttrData(listStruct, listItem, listAtom);

        return result;
    }

    //
    // ATTRID
    //
    itemtype = listStruct->at(0);
    if (itemtype == NULL)
    {
        TRACE_ERROR((_TX("Illegal message struct.\n")));
        atom->dump();
        return false;
    }
    result = itemtype->availFormat(atom->format() & ~ATOM_ARRAY);
    if (! result)
    {
        ACE_ERROR((LM_ERROR,
            "BS2Message::parseAttrIDAndData: illegal item format (%s 0x%X).\n",
             itemtype->itemName().c_str(), (atom->format() & ~ATOM_ARRAY)));
        m_complete = 0;
        return false;          // Error-Log
    }

    BS2Item * item = itemtype->factory(atom);
    listItem->items().push_back(item);

    //
    // ATTRDATA
    //
    for (size_t i = 1; i < atoms.size(); i++)
    {
        itemtype = listStruct->at(i);
        if (itemtype == NULL)
        {
            TRACE_ERROR((_TX("Illegal message struct(%d).\n"), i));
            atom->dump();
            return false;
        }
        atom = reinterpret_cast<BS2Atom *>(atoms[i]);
        result = itemtype->availFormat(atom->format() & ~ATOM_ARRAY);
        if (! result)
        {
            ACE_ERROR((LM_ERROR,
                "BS2Message::parseAttrIDAndData: illegal item format (%s 0x%X).\n",
                itemtype->itemName().c_str(), (atom->format() & ~ATOM_ARRAY)));
            m_complete = 0;
            return false;          // Error-Log
        }
        item = (BS2ListItem *)itemtype->factory(atom);
        listItem->items().push_back(item);
        if (item->isList())
        {
            BS2ListItem * list_item = (BS2ListItem *)item;
            BS2List * list_atom = (BS2List *)atom;
            if (! parseAttrData(listStruct, list_item, list_atom))
            {   // Reset type   ^^^^^^^^^^
                delete list_atom;
                list_item->clearAtom();
                return false;
            }
            delete list_atom;
            list_item->clearAtom();
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// input object attribute element (only ATTRDATA)
//-----------------------------------------------------------------------------
bool BS2Message::parseAttrData(BS2Itemtypes * listStruct,
                               BS2ListItem * listItem,
                               BS2List * listAtom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::parseAttrData");
    BS2ItemType * itemtype;
    BS2Atom * atom;

    unsigned int atomq = listAtom->getList().size();
    if (atomq == 0)
    {
        return true;          // no item in list
    }

    itemtype = listStruct->at(1);    // Get ATTRDATA's item type
    if (itemtype == NULL)
    {
        TRACE_ERROR((_TX("Illegal item type struct(ATTRDATA).\n")));
        return false;
    }

    BS2Atoms& atoms = listAtom->getList();
    for (unsigned int i = 0; i < atomq; i++)
    {
        atom = reinterpret_cast<BS2Atom *>(atoms[i]);
        if (atom->isList())
        {
            // Parse ATTRID + ATTRDATA
            BS2ListItem * list_item = (BS2ListItem *)itemtype->factory(atom);
            listItem->items().push_back(list_item);
            BS2List * list_atom = (BS2List *)atom;
            if (! parseAttrIDAndData(listStruct, list_item, list_atom))
            {   // Reset type        ^^^^^^^^^^
                delete list_atom;
                list_item->clearAtom();
                return false;
            }
            delete list_atom;
            list_item->clearAtom();
        }
        else
        {   // data only
            bool result = itemtype->availFormat(atom->format() & ~ATOM_ARRAY);
            if (! result)
            {
                ACE_ERROR((LM_ERROR,
                    "BS2Message::attrElements : illegal item format (%s 0x%X).\n",
                    itemtype->itemName().c_str(), (atom->format() & ~ATOM_ARRAY)));
                m_complete = 0;
                return false;          // Error-Log
            }

            BS2Item * item = itemtype->factory(atom);
            listItem->items().push_back(item);
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// Convert atom tree to item tree (message)
//-----------------------------------------------------------------------------
int BS2Message::atomToItem(BS2AtomList * atoms)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::atomToItem");
    BS2ItemType * itemtype;
    BS2Atom * atom;

    size_t itemCount = 0;
    m_complete = 1;
    if (atoms != NULL)
    {
        if (itemCount == m_msgtype->body().size())
            throw runtime_error(_TX("BS2Message::atomToItem()"));

        int atomq = atoms->size();
        for (int i = 0; i < atomq; i++)
        {
            itemtype = m_msgtype->body().at(itemCount);
            atom = atoms->front();
            atoms->pop_front();
            if (itemtype->availFormat(atom->format() & ~ATOM_ARRAY))
            {
                BS2Item * clone = itemtype->factory(atom);
                m_items.push_back(clone);
                if (clone->isList())
                {
                    BS2ListItem * list_item = (BS2ListItem *)clone;
                    BS2List * list_atom = (BS2List *)atom;
                    if (listElements((BS2Itemtypes *)itemtype, list_item,
                                     list_atom) == false)
                    {
                        delete list_atom;
                        list_item->clearAtom();
                        break;
                    }
                    delete list_atom;
                    list_item->clearAtom();
                }
            }
            itemCount++;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Create reply object (static)
//-----------------------------------------------------------------------------
BS2Message * BS2Message::response(const BS2Message * msg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::response");
    if (msg->isSecondary())
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("%s : Illegal message (%s).\n"),
                             "BS2Message::response", msg->charName()));
        return NULL;
    }
    int sf = (msg->sf() & 0x7FFF) + 1;   // primary to secondary
    BS2Message * result = BS2MessageDictionary::instance()->factory(sf);
    if (result == NULL)
    {
        ACE_ERROR((LM_ERROR, ACE_TEXT("%s : Illegal stream/function (%s).\n"),
                             "BS2Message::response", msg->charName()));
        return result;
    }
    result->m_transactionNum = msg->m_transactionNum;
    return result;
}

//-----------------------------------------------------------------------------
BS2Message * BS2Message::response(const BS2Message * msg, const BYTE ack,
                                  const BCHAR * iname)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::response");
    BS2Message * result = BS2Message::response(msg);
    if (result == NULL)
    {
        return result;
    }
    BS2Binary * atom = new BS2Binary(ack);
    BS2Item * ackItem = BS2Item::factory((BCHAR *)iname, atom);
    result->add(ackItem);

    return result;
}

//-----------------------------------------------------------------------------
BS2Message * BS2Message::response(const BS2Message * msg, UINT ack,
                                  const BCHAR * iname)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::response");
    BS2Message * result = BS2Message::response(msg);
    if (result == NULL)
    {
        return result;
    }
    BS2UInt1 * atom = new BS2UInt1((BYTE)(ack & 0xFF));
    BS2Item * ackItem = BS2Item::factory((BCHAR *)iname, atom);
    result->add(ackItem);

    return result;
}

//-----------------------------------------------------------------------------
// Make multi-block inquire message (static)
//-----------------------------------------------------------------------------
BS2Message * BS2Message::makeInquiry(const BS2Message * constmsg)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::makeInquiry");
    BS2Message * msg = const_cast<BS2Message *>(constmsg);
    BS2Message * result = BS2Message::factory(msg->inqNum());
    if (result == NULL)
    {
        TRACE_ERROR((_TX("Illegal multi-block inquire (0x%X).\n"),
                     msg->inqNum()));
        return NULL;
    }
    int length = msg->dataSize();
    int sfnum  = msg->sf();
    if (length < 245 &&
        (! (sfnum == SFCODE(12,7) || sfnum == SFCODE(12,1) ||
            sfnum == SFCODE(12,11))))
    {   // One block size
        return NULL;
    }

    BYTE mapft;
    BS2Atom * clone;
    BS2ListItem * rootlist = new BS2ListItem;
    switch (sfnum)
    {
#if 0
    case SFCODE(0,0):
        clone = BS2Atom::factory(msg->getAtom(_TX("SPID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("SPID"), clone));
        rootlist->add(BS2Item::factory(_TX("LENGTH"), new BS2Int4(length)));
        break;
#endif
    case SFCODE(2,23):   // S2F39
    case SFCODE(2,33):
    case SFCODE(2,35):
    case SFCODE(2,45):
    case SFCODE(2,49):

    case SFCODE(6,3):    // S3F5
    case SFCODE(6,9):
    case SFCODE(6,11):
    case SFCODE(6,13):

    case SFCODE(16,11):  // S16F1
    case SFCODE(16,13):
    case SFCODE(16,15):
        clone = BS2Atom::factory(msg->getAtom(_TX("DATAID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("DATAID"), clone));
        rootlist->add(BS2Item::factory(_TX("DATALENGTH"), new BS2Int4(length)));
        break;
    case SFCODE(7,3):    // S7F1
    case SFCODE(7,23):
    case SFCODE(7,31):
        clone = BS2Atom::factory(msg->getAtom(_TX("PPID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("DATAID"), clone));
        rootlist->add(BS2Item::factory(_TX("LENGTH"), new BS2Int4(length)));
        break;
#if 0
    case SFCODE(0,0):    // S7F29
        result->add(BS2Item::factory(_TX("LENGTH"), new BS2Int4(length)));
        delete rootlist;
        rootlist = NULL;
        break;
#endif
    case SFCODE(12,7):   // S12F5
    case SFCODE(12,9):
    case SFCODE(12,11):
        if (sfnum == SFCODE(12,7))
        {
            mapft = 0;       // Row format
        }
        else if (sfnum == SFCODE(12,9))
        {
            mapft = 2;       // Coordinate format
        }
        else if (sfnum == SFCODE(12,9))
        {
            mapft = 1;       // Array format
        }
        clone = BS2Atom::factory(msg->getAtom(_TX("MID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("MID"), clone));
        clone = BS2Atom::factory(msg->getAtom(_TX("IDTYP")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("IDTYP"), clone));
        rootlist->add(BS2Item::factory(_TX("MAPFT"), new BS2Binary(mapft)));
        rootlist->add(BS2Item::factory(_TX("MLCL"), new BS2Int4(length)));
        break;
    case SFCODE(15,13):    // S15F1
    case SFCODE(15,15):
    case SFCODE(15,23):
    case SFCODE(15,25):
    case SFCODE(15,27):
    case SFCODE(15,29):
    case SFCODE(15,33):
    case SFCODE(15,35):
    case SFCODE(15,39):
    case SFCODE(15,41):
    case SFCODE(15,43):
    case SFCODE(15,45):
    case SFCODE(15,47):
        clone = BS2Atom::factory(msg->getAtom(_TX("DATAID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("DATAID"), clone));

        if (sfnum == SFCODE(15,13) || sfnum == SFCODE(15,15) ||
            sfnum == SFCODE(15,27) || sfnum == SFCODE(15,45))
        {
            clone = BS2Atom::factory(msg->getAtom(_TX("RCPSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,23))
        {
            clone = BS2Atom::factory(msg->getAtom(_TX("OBJSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,25))
        {
            clone = BS2Atom::factory(msg->getAtom(_TX("RMNSSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(15,29) || sfnum == SFCODE(15,33) ||
                 sfnum == SFCODE(15,35))
        {
            clone = BS2Atom::factory(msg->getAtom(_TX("RESPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,39))
        {                                      // "RMSEGSPEC" ?
            clone = BS2Atom::factory(msg->getAtom(_TX("RMRECSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,41))
        {
            clone = BS2Atom::factory(msg->getAtom(_TX("RMRECSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,43))
        {                                      // "TARGETSPEC" ?
            clone = BS2Atom::factory(msg->getAtom(_TX("OBJSPEC")));
            BEEAssert(clone != NULL);
        }
        else if (sfnum == SFCODE(12,43))
        {                        // "RMNSSPEC" or "RMRECSPEC" ?
            clone = BS2Atom::factory(msg->getAtom(_TX("OBJSPEC")));
            BEEAssert(clone != NULL);
        }
        rootlist->add(BS2Item::factory(_TX("RCPSPEC"), clone));
        rootlist->add(BS2Item::factory(_TX("RMDATASIZE"), new BS2Int4(length)));
        break;
    case SFCODE(15,37):    // S15F1
        clone = BS2Atom::factory(msg->getAtom(_TX("RCPID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("DATAID"), clone));
        clone = BS2Atom::factory(msg->getAtom(_TX("RMSEGSPEC")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("RCPSPEC"), clone));
        rootlist->add(BS2Item::factory(_TX("RMDATASIZE"), new BS2Int4(length)));
        break;
    default:           // S3F15, S4F25
        clone = BS2Atom::factory(msg->getAtom(_TX("DATAID")));
        BEEAssert(clone != NULL);
        rootlist->add(BS2Item::factory(_TX("DATAID"), clone));
        rootlist->add(BS2Item::factory(_TX("DATALENGTH"), new BS2Int4(length)));
        break;
    }
    if (rootlist != NULL)
    {
        result->add(rootlist);
    }
    return result;
}

//-----------------------------------------------------------------------------
// add item
//-----------------------------------------------------------------------------
void BS2Message::add(BS2Item * item)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::add");
    if (item == NULL)
        return ;
    m_items.push_back(item);
    if (item->isList())
    {
        m_itemCount += ((BS2ListItem *)item)->itemCount();
        m_dataSize += ((BS2ListItem *)item)->size();
    }
    else
    {
        m_itemCount++;
        m_dataSize += item->size();
    }
}

//-----------------------------------------------------------------------------
// Traverse message table
//-----------------------------------------------------------------------------
static int _traverse(ClientProcedure proc, void * clientData,
                     BS2ListItem * aList)
{
    TRACE_FUNCTION(TRL_LOW, "_traverse");
    BS2Item * item;
    BS2Items::iterator curIter;
    int result;

    for (curIter = aList->items().begin();
         curIter != aList->items().end(); ++curIter)
    {
        item = *curIter;
        if (proc != NULL)
        {
            result = proc(clientData, item);
            if (result != 0)
                return result;
        }
        if (item->isList())
        {
            result = _traverse(proc, clientData, (BS2ListItem *)item);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Message::traverse(ClientProcedure proc, void * clientData)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::traverse");
    BS2Item * item;
    IterStack     listIter;
    BS2ItemList::iterator topIter;
    int result;

    for (topIter = m_items.begin(); topIter != m_items.end();
         ++topIter)
    {
        item = *topIter;
        if (proc != NULL)
        {
            result = proc(clientData, item);
            if (result != 0)
                return result;
        }
        if (item->isList())
        {
            result = _traverse(proc, clientData, (BS2ListItem *)item);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Traverse message table with traverser
//-----------------------------------------------------------------------------
static int _traverse(BS2Traverser * userp, BS2ListItem * aList)
{
    TRACE_FUNCTION(TRL_LOW, "_traverse");
    BS2Item * item;
    BS2Items::iterator curIter;
    int result;

    for (curIter = aList->items().begin();
         curIter != aList->items().end(); ++curIter)
    {
        item = *curIter;
        if (item->isListItem())
        {
            result = userp->beginList((BS2ListItem *)item);
            if (result != 0)
                return result;
            result = _traverse(userp, (BS2ListItem *)item);
            if (result != 0)
                return result;
            result = userp->endList((BS2ListItem *)item);
            if (result != 0)
                return result;
        }
        else if (item->isList())
        {
            result = userp->beginValueList(item);
            if (result != 0)
                return result;

            result = userp->endValueList(item);
            if (result != 0)
                return result;
        }
        else
        {
            result = userp->parseItem(item);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Message::traverse(BS2Traverser * userp)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::traverse");
    BS2Item * item;
    IterStack     listIter;
    BS2ItemList::iterator topIter;
    int result;

    for (topIter = m_items.begin(); topIter != m_items.end();
         ++topIter)
    {
        item = *topIter;
        if (item->isList())
        {
            result = userp->beginList((BS2ListItem *)item);
            if (result != 0)
                return result;
            result = _traverse(userp, (BS2ListItem *)item);
            if (result != 0)
                return result;
            result = userp->endList((BS2ListItem *)item);
            if (result != 0)
                return result;
        }
        else
        {
            result = userp->parseItem(item);
            if (result != 0)
                return result;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
// input list element
//-----------------------------------------------------------------------------
int BS2Message::parseList(BS2ListItem * listStruct, BS2ListItem * listItem,
                          BS2List * listAtom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::parseList");
    BS2Item * item;
    BS2Atom * atom;

    BS2Atoms& atoms = listAtom->getList();
    int atomq = atoms.size();
    BS2Items::iterator listItemIter = listStruct->items().begin();
    for (int i = 0; i < atomq; i++)
    {
        item = *listItemIter;
        atom = reinterpret_cast<BS2Atom *>(atoms[i]);
        bool result = item->availFormat(atom->format() & ~ATOM_ARRAY);
        if (result)
        {
            BS2Item * clone = item->clone(atom);
            listItem->items().push_back(clone);
            if (item->isList())
            {
                BS2ListItem * list_item = (BS2ListItem *)clone;
                BS2List * list_atom = (BS2List *)atom;
                if (parseList((BS2ListItem *)item, list_item, list_atom) < 0)
                {
                    delete list_atom;
                    list_item->clearAtom();
                    return -1;
                }
                delete list_atom;
                list_item->clearAtom();
            }
        }
        else
        {
            ACE_ERROR((LM_ERROR, "BS2Message::parseList : illegal item format (%s 0x%X).\n",
                                  item->charName(), (atom->format() & ~ATOM_ARRAY)));
            m_complete = 0;
            return -1;          // Error-Log
        }
        ++listItemIter;
        if (listItemIter == listStruct->items().end())
            listItemIter = listStruct->items().begin();
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Get GRANT(GRANT6,PPGNT,GRNT1,RMGRNT) code
//-----------------------------------------------------------------------------
int BS2Message::getGrant()
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::getGrant");
    unsigned int result;
    BCHAR * gname = "GRANT";
    if (sf() == SFCODE(6,6))
    {
        gname = "GRANT6";
    }
    else if (sf() == SFCODE(12,6))
    {
        gname = "GRNT1";
    }
    else if (sf() == SFCODE(7,2) || sf() == SFCODE(7,30))
    {
        gname = "PPGNT";
    }
    else if (sf() == SFCODE(15,2) || sf() == SFCODE(15,37) ||
             sf() == SFCODE(15,46))
    {
        gname = "RMGRNT";
    }
#if 1
    else if (! (sf() == SFCODE(2,2) || sf() == SFCODE(2,40) ||
                sf() == SFCODE(3,16) || sf() == SFCODE(4,26) ||
                sf() == SFCODE(13,12) || sf() == SFCODE(14,24) ||
                sf() == SFCODE(16,2) || sf() == SFCODE(16,12)))
    {
        ACE_ERROR((LM_ERROR,
                  ACE_TEXT("BS2Message::getGrant : illegal stream/function (%s).\n"),
                  this->charName()));
    }
#endif

    BS2Atom * atom = this->getAtom(gname);
    if (atom != NULL)
    {                          // GRANT code
        return 2;              // 1 = Busy, Try again
                               // 2 = No Space Available
                               // 3 = Duplicate DATAID
    }
    if (! atom->isBinary())
    {
        return 2;
    }
    result = *((BS2Binary *)atom)->value();    // Convert format
    return (int)result;
}

//-----------------------------------------------------------------------------
// Get item value by name
//-----------------------------------------------------------------------------
class Taverser_GetItem : public BS2Traverser
{
public:
    Taverser_GetItem(const BCHAR * name) : m_iname(name), m_item(NULL) {}
    virtual ~Taverser_GetItem() {}

    virtual int parseItem(BS2Item * item) 
    {
        if (m_iname == item->name())
        {
            this->m_item = item;
            return 1;
        }
        return 0;
    }

    string  m_iname;
    BS2Item * m_item;
};

//-----------------------------------------------------------------------------
BS2Item * BS2Message::getItem(const BCHAR * iname)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::getItem");

    Taverser_GetItem result(iname);
    int rv = this->traverse(&result);
    if (rv < 0)
    {   // error
        return NULL;
    }

    return result.m_item;   // Found atom or NULL
}


//-----------------------------------------------------------------------------
// Get atom value by name
//-----------------------------------------------------------------------------
class Taverser_GetAtom : public BS2Traverser
{
public:
    virtual ~Taverser_GetAtom() {}

    virtual int parseItem(BS2Item * item) {
            if (m_iname == item->name()) {
                this->m_atom = item->atom();
                return 1;
            }
            return 0;
        }
    string  m_iname;
    BS2Atom * m_atom;
};

//-----------------------------------------------------------------------------
BS2Atom * BS2Message::getAtom(const BCHAR * iname)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::getAtom");

    Taverser_GetAtom result;

    result.m_iname = iname;
    result.m_atom = NULL;
    int rv = this->traverse(&result);
    if (rv < 0)
    {   // error
        return NULL;
    }

    return result.m_atom;   // Found atom or NULL
}

//-----------------------------------------------------------------------------
// Get item value by name
//-----------------------------------------------------------------------------
class Taverser_GetValues : public BS2Traverser
{
public:
    virtual ~Taverser_GetValues() {}

    virtual int parseItem(BS2Item * item) {
            if (m_iname == item->name()) {
                b_value curval;
                item->atom()->get(curval);
                this->m_values->push_back(curval);
            }
            return 0;
        }

    string  m_iname;
    vector<b_value> * m_values;
};

//-----------------------------------------------------------------------------
int BS2Message::getValue(const BCHAR * iname, vector<b_value>& retval)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::getValue");

    Taverser_GetValues result;

    result.m_iname = iname;
    result.m_values = &retval;
    int rv = this->traverse(&result);
    if (rv < 0)
    {   // error
        return rv;
    }
    return retval.size();   // Found atom number
}

//-----------------------------------------------------------------------------
// Get item id by name
//-----------------------------------------------------------------------------
class Taverser_GetID : public BS2Traverser
{
public:
    virtual ~Taverser_GetID() {}

    virtual int parseItem(BS2Item * item) {
            if (m_iname == item->name()) {
                b_id curid;
                item->atom()->get(curid);
                this->m_ids->push_back(curid);
            }
            return 0;
        }

    string  m_iname;
    vector<b_id> * m_ids;
};

//-----------------------------------------------------------------------------
int BS2Message::getId(const BCHAR * iname, vector<b_id>& retid)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::getId");

    Taverser_GetID result;

    result.m_iname = iname;
    result.m_ids = &retid;
    int rv = this->traverse(&result);
    if (rv < 0)
    {   // error
        return rv;
    }
    return retid.size();   // Found atom number
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2Message::print(BS2InterpBase * interp) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::print");
    UINT trnum = 0;
    if (interp->print_xml())
    {
        // Print transaction data
        interp->printf(_TX("<secs:message "));
        interp->printf(_TX("sf=\"%s\" "), charName());
        interp->printf(_TX("wait=\"%s\" "), (this->m_wbit) ? _TX("TRUE") : _TX("FALSE"));
        trnum = this->transNum();
        interp->printf(_TX("transaction=\"%u\">\n"), trnum);
        interp->printf(_TX(" <body>"));

        // Message body
        if (m_items.size() > 0)
        {
            BS2ItemList::const_iterator iter;
            for (iter = m_items.begin(); iter != m_items.end(); ++iter)
            {
                BS2Item * item = *iter;
                item->print(interp);
            }
        }
        interp->printf(_TX("  </body>\n"));
        interp->printf(_TX("</secs:message> "));
    }
    else
    {
        // Print transaction data
        trnum = this->transNum();
        if (m_wbit)
            interp->printf(_TX("MESSAGE {0x%X WAIT} "), trnum);
        else
            interp->printf(_TX("MESSAGE {0x%X} "), trnum);

        // Print body
        interp->printf(_TX("%s "), charName());
        if (m_items.size() > 0)
        {
            BS2ItemList::const_iterator iter;
            for (iter = m_items.begin(); iter != m_items.end(); ++iter)
            {
                BS2Item * item = *iter;
                item->print(interp);
            }
        }
        interp->printf(_TX(" \n"));
    }
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2Message::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2Message::dump");

    b_printf(_TX("%s "), charName());
    if (m_items.size() > 0)
    {
        BS2ItemList::const_iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            BS2Item * item = *iter;
            item->dump();
        }
    }
    b_printf(_TX(" \n"));
}

//add by slime
void BS2Message::EraseAtoms_Recursive(BS2List* list_atom)
{
	unsigned int					i, atomq;
	BS2Atoms						atoms;
	BS2Atom*						atom;
	
	if(list_atom->size() > 0)
	{
		atoms	= list_atom->getList();
		atomq	= atoms.size();
		
		for(i = 0; i < atomq; i++)
		{
			atom = reinterpret_cast<BS2Atom *>(atoms[i]);

			if(atom->isList())
			{
				EraseAtoms_Recursive((BS2List*)atom);
			}
			else if(atom->isArray())
			{
				//nothing
			}
			else
			{
				if(atom)
					delete atom;
			}
		}
	}
}
//
// *** End of File ***
