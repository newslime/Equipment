// $Id: BS2MessageType.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

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

#include "BS2MessageType.h"
#include "BS2Message.h"
#include "BS2ListItem.h"
#include "BS2Stream.h"
#include "BS2Interpreter.h"


//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2MessageType::BS2MessageType(UINT sf, int dir, bool wait, const string& mngr,
                               UINT multi)
        : m_sfcode(sf), m_dir(dir), m_wait(wait), m_mngrName(mngr)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2MessageType::BS2MessageType");

    BCHAR buf[128];
    _stprintf(buf, _TX("S%dF%d"), STREAMCODE(sf), FUNCCODE(sf));
    m_name = buf;
    m_multi = multi;
    if (multi < MULTI_BLOCK_MESSAGE && multi != SINGLE_BLOCK_MESSAGE)
    {
        m_inqNum = multi;
        m_multi = MULTI_BLOCK_MESSAGE;
    }
}

//-----------------------------------------------------------------------------
// add item of message type
//-----------------------------------------------------------------------------
void BS2MessageType::add(BS2ItemType * item)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageType::add");
    if (item == NULL)
        return ;

    m_itemMember.add(item);
}

//-----------------------------------------------------------------------------
void BS2MessageType::add(BS2Itemtypes * items)
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageType::add");
    
	if (items == NULL)
        return ;

    m_itemMember.add((BS2ItemType *)items);
}

//-----------------------------------------------------------------------------
void BS2MessageType::Erase_Recursive(BS2ItemType* item)
{
	unsigned int					i;
	unsigned int					size;
	BS2Itemtypes*					types;
	vector<BS2ItemType *>::iterator	vit;
	
	types	= (BS2Itemtypes*)item;

	vit		= types->m_items.begin();
	size	= types->m_items.size();

	for(i=0; i<size; i++)
	{
		item = *vit;
		
		if( item->isList() && !item->isObject() )
		{
			Erase_Recursive(item);
			delete item;
		}
											
		types->m_items.erase(vit);
		vit = types->m_items.begin();
	}
}


void BS2MessageType::EraseList()
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageType::add");
    
	unsigned int					i;
	unsigned int					size;
	BS2ItemType*					item;
	vector<BS2ItemType *>::iterator	vit;

	if( m_itemMember.m_items.size() > 0 )
	{
		vit		= m_itemMember.m_items.begin();
		size	= m_itemMember.m_items.size();
		
		for(i=0; i<size; i++)
		{
			item = *vit;
			
			if( item->isList() && !item->isObject() )
			{
				Erase_Recursive(item);
				delete item;
			}
			
			m_itemMember.m_items.erase(vit);
			vit = m_itemMember.m_items.begin();
		}
	}
}

//-----------------------------------------------------------------------------
// input secs2 stream function
//-----------------------------------------------------------------------------
BS2Message * BS2MessageType::factory(const BS2IStream * buf) const
{
    TRACE_FUNCTION(TRL_LOW, "BS2MessageType::factory");
    // inq stream function
    BS2Message* new_msg = new BS2Message(m_sfcode);
    new_msg->m_msgtype = const_cast<BS2MessageType *>(this);
    new_msg->m_wbit = m_wait;
    
	if (buf != NULL)
    {
        if (new_msg->factory_base(const_cast<BS2IStream *>(buf)) < 0)
        {
            delete new_msg;
            return NULL;
        }
    }
    
	return new_msg;
}

//-----------------------------------------------------------------------------
// Print message type
//-----------------------------------------------------------------------------
void BS2MessageType::print(BS2InterpBase * interp) const
{
    size_t itemCount;
    if (interp->print_xml())
    {
        interp->printf(_TX("<streamFunction>%s</streamFunction> "),
                       this->charName());
        if (m_itemMember.size() > 0)
        {
            for (itemCount = 0; itemCount < m_itemMember.size(); itemCount++)
            {
                BS2ItemType * itemtype = m_itemMember.at(itemCount);
                itemtype->print(interp);
            }
        }
        interp->printf(_TX(" \n"));
    }
    else
    {
        interp->printf(_TX("%s "), charName());
        if (m_itemMember.size() > 0)
        {
            for (itemCount = 0; itemCount < m_itemMember.size(); itemCount++)
            {
                BS2ItemType * itemtype = m_itemMember.at(itemCount);
                itemtype->print(interp);
            }
        }
        interp->printf(_TX(" \n"));
    }
}

//-----------------------------------------------------------------------------
// Dump message type
//-----------------------------------------------------------------------------
void BS2MessageType::dump() const
{
    if (m_itemMember.size() > 0)
    {
        size_t itemCount;
        for (itemCount = 0; itemCount < m_itemMember.size(); itemCount++)
        {
            BS2ItemType * itemtype = m_itemMember.at(itemCount);
            itemtype->dump();
        }
    }
}


//
// *** End of File ***
