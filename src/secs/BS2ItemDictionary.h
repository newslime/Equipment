// $Id: BS2ItemDictionary.h,v 1.8 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ItemDictionary.h
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

#ifndef BS2ITEMDICTIONARY_H
#define BS2ITEMDICTIONARY_H

#include "BS2ItemType.h"
#include "BS2Item.h"

typedef map<string, BS2ItemType *, str_less, allocator<BS2ItemType *> >
        BS2ItemMap;

//
class BS2Atom;

//-----------------------------------------------------------------------------
class BEE_Export BS2ItemDictionary
{
public:
    BS2ItemDictionary() { }
    ~BS2ItemDictionary();

    // Get pointer to <BS2ItemDictionary>.
    static BS2ItemDictionary* instance(void);
    static void close_singleton(void);

    bool  exist(string& name);
    BS2ItemType * find(string& name) { return find(name.c_str()); }
    BS2ItemType * find(const BCHAR * name);
    BS2ItemType * add(const string& name, UINT format,
                      UINT defaultFormat = 0xFFFFF, bool objOrNot = false);
    int       erase(string& name) { return erase(name.c_str()); }
    int       erase(const BCHAR * name);
    void      eraseAll();
    BS2Item * factory(const string& name, BS2Atom * atom = NULL) {
            return factory(name.c_str(), atom);
        }
    BS2Item * factory(const BCHAR * name, BS2Atom * atom = NULL);

    void print(BS2InterpBase * interp);
    void dump() const;

protected:
    int    close(void);

protected:
    // = ACE_Thread_Mutex and condition variable for synchronizing termination.
    ACE_Recursive_Thread_Mutex m_lock;

private:
    BS2ItemMap m_items;

    // Pointer to a process-wide <BS2ItemDictionary>.
    static BS2ItemDictionary * _itemdict;

    // Must delete the <_item_dict> if non-0.
    static int _delete_itemdict;
};


#endif
