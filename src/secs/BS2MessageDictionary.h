// $Id: BS2MessageDictionary.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2MessageDictionary.h
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

#ifndef BS2MESSAGEDICTIONARY_H
#define BS2MESSAGEDICTIONARY_H

#include "BS2Message.h"

#define USE_MULTIMAP

#ifdef USE_MULTI_MAP
typedef multimap<UINT, BS2MessageType *, uint_less, allocator<BS2MessageType *> > BS2Messages;
#else
typedef map<UINT, BS2MessageType *, uint_less, allocator<BS2MessageType *> > BS2Messages;
#endif

//---------------------------------------------------------------------------------------
class BEE_Export BS2MessageDictionary
{
public:
    BS2MessageDictionary() { }
    ~BS2MessageDictionary();

    // Get pointer to <BS2MessageDictionary>.
    static		BS2MessageDictionary * instance(void);
    static void	close_singleton(void);
    
	int  close(void);
    bool init();
    bool existStreamNum(int n);
    BS2MessageType * search(UINT sf);
    BS2Message * factory(UINT sf);
    BS2Message * make(BS2OStream * buf);
    BS2MessageType * add(BS2MessageType * type);
    int  erase(UINT sfnum);
    void eraseAll();
    void getSpoolingStreams(StreamFunctionNumbers& sfnums);

    void print(BS2InterpBase * interp);
    void dump() const;

protected:
    // = ACE_Thread_Mutex and condition variable for synchronizing termination.
    ACE_Recursive_Thread_Mutex m_lock;

private:
    BS2Messages m_messages;

    // Pointer to a process-wide <BS2MessageDictionary>.
    static BS2MessageDictionary * _msg_dict;

    // Must delete the <_msg_dict> if non-0.
    static int _delete_msg_dict;
};


#endif
