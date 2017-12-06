// $Id: BS2ItemDictionary.cpp,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ItemDictionary.cpp
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

#include "BS2Stream.h"
#include "BS2ItemDictionary.h"
#include "BS2ItemType.h"
#include "BS2Interpreter.h"

typedef BS2ItemMap::value_type BS2ItemPair;

// Process-wide Item Dictionary.
BS2ItemDictionary * BS2ItemDictionary::_itemdict = NULL;

// Controls whether the BS2ItemDictionary is deleted when we shut down
int BS2ItemDictionary::_delete_itemdict = 0;

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2ItemDictionary::~BS2ItemDictionary()
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemDictionary::BS2ItemDictionary");

    this->close();
}

//-----------------------------------------------------------------------------
// Get Item Dictonary.
//-----------------------------------------------------------------------------
BS2ItemDictionary * BS2ItemDictionary::instance(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::instance");

    if (BS2ItemDictionary::_itemdict == 0)
    {   // Perform Double-Checked Locking Optimization.
        
//#if defined(_WIN32)		
//		ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance(), 0));
//#endif

        if (BS2ItemDictionary::_itemdict == 0)
        {
            ACE_NEW_RETURN(BS2ItemDictionary::_itemdict, BS2ItemDictionary, 0);
            BS2ItemDictionary::_delete_itemdict = 1;
        }
    }
    return BS2ItemDictionary::_itemdict;
}

//-----------------------------------------------------------------------------
void BS2ItemDictionary::close_singleton(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::close_singleton");

//#if defined(_WIN32)	
//    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance()));
//#endif

    if (BS2ItemDictionary::_delete_itemdict)
    {
        BS2ItemDictionary::_itemdict->close();
        delete BS2ItemDictionary::_itemdict;
        BS2ItemDictionary::_itemdict = NULL;
        BS2ItemDictionary::_delete_itemdict = 0;
    }
}

//------------------------------------------------------------------------------
// Close up and release all resources.
//------------------------------------------------------------------------------
int BS2ItemDictionary::close(void)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::close");

    // Clean up the thread descriptor list.
    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, -1));

    BS2ItemType * item;
    if (m_items.size() > 0)
    {
        BS2ItemMap::iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            item = (*iter).second;
            delete item;
        }
        m_items.erase(m_items.begin(), m_items.end());
    }

    return 0;
}

//-----------------------------------------------------------------------------
// check to support stream number.
//-----------------------------------------------------------------------------
bool BS2ItemDictionary::exist(string& name)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2ItemDictionary::existItem");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2ItemMap::iterator iter = m_items.find(name);
    if (iter == m_items.end())
    {
        return false;
    }
    BS2ItemType * item = (*iter).second;
    return (item != NULL);
}

//-----------------------------------------------------------------------------
// Find Item by name.
//-----------------------------------------------------------------------------
BS2ItemType * BS2ItemDictionary::find(const BCHAR * name)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::find");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));
    string nameStr = name;
    BS2ItemMap::iterator iter = m_items.find(nameStr);
    if (iter == m_items.end())
    {
        return NULL;
    }
    BS2ItemType * item = (*iter).second;
    return item;
}

//-----------------------------------------------------------------------------
// Add new item.
//-----------------------------------------------------------------------------
BS2ItemType * BS2ItemDictionary::add(const string& name, UINT formats,
                                     UINT defaultFormat, bool isObj)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::add");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2ItemType * item = NULL;
    BS2ItemMap::iterator iter = m_items.find(name);
    if (iter != m_items.end())
    {
        item = (*iter).second;
        item->formats(formats);              // change format data
        item->defaultFormat(defaultFormat);  // change default format data
        item->setObject(isObj);              //
    }
    else
    {
        if ((formats & BS2ItemType::LIST_BIT) && isObj)
        {   // Object's member item
            item = new BS2Itemtypes();
            item->itemName(name);
            item->formats(formats);
            item->defaultFormat(defaultFormat);
            item->setObject();
            m_items.insert(BS2ItemPair(name, item));
        }
        else
        {
            item = new BS2ItemType(name, formats, defaultFormat);
            m_items.insert(BS2ItemPair(name, item));
        }
    }
    return item;
}

//-----------------------------------------------------------------------------
// Erase item.
//-----------------------------------------------------------------------------
int BS2ItemDictionary::erase(const BCHAR * name)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::erase");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2ItemType * item = NULL;
    BS2ItemMap::iterator iter = m_items.find(name);
    if (iter != m_items.end())
    {
        item = (*iter).second;
        m_items.erase(iter);
        delete item;
    }
    return 0;
}

//-----------------------------------------------------------------------------
void BS2ItemDictionary::eraseAll()
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::eraseAll");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock));

    BS2ItemType* item;
    /*if (m_items.size() > 0)
    {
        BS2ItemMap::iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            item = (*iter).second;
            delete item;
            m_items.erase(iter);
        }
    }
    
	return;*/

	unsigned int			i;
	unsigned int			size;
	BS2ItemMap::iterator	iter;
		
	if ( m_items.size() > 0 )
    {
		iter = m_items.begin();
		size = m_items.size();
		
		for(i=0; i<size; i++)
		{
			item = (*iter).second;
			delete item;
			
			m_items.erase(iter);
			iter = m_items.begin();
		}
    }
}


//-----------------------------------------------------------------------------
// Create Item object
//-----------------------------------------------------------------------------
BS2Item * BS2ItemDictionary::factory(const BCHAR * name, BS2Atom * atom)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::factory");

    ACE_MT(ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock, 0));

    BS2ItemType * itemType = this->find(name);
    if (itemType == NULL)
    {
        ACE_ERROR((LM_ERROR,
                  "BS2ItemDictionary::factory : illegal item name (%s).\n",
                  name));
        return NULL;
    }
    BS2Item * item = new BS2Item(itemType);
    item->setAtom(atom);
    return item;
}

//-----------------------------------------------------------------------------
// Print
//-----------------------------------------------------------------------------
void BS2ItemDictionary::print(BS2InterpBase * interp)
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::print");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon, this->m_lock));

    BS2ItemType * item;
    if (m_items.size() > 0)
    {
        BS2ItemMap::const_iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            item = (*iter).second;
            item->print(interp);
        }
    }
    return ;
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void BS2ItemDictionary::dump() const
{
    TRACE_FUNCTION(TRL_LOW, "BS2ItemDictionary::dump");

    ACE_MT(ACE_GUARD(ACE_Recursive_Thread_Mutex, ace_mon,
                     const_cast<ACE_Recursive_Thread_Mutex&>(this->m_lock)));

    BS2ItemType * item;
    if (m_items.size() > 0)
    {
        BS2ItemMap::const_iterator iter;
        for (iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            item = (*iter).second;
            item->dump();
        }
    }

    return ;
}


//
// *** End of File ***

