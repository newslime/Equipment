// $Id: b_ace.h,v 1.7 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_ace.h
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

#ifndef B_ACE_H
#define B_ACE_H

#ifdef _MSC_VER
#pragma warning(disable: 4786 4284 4018)
#endif

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

#include "ace/Version.h"
#include "ace/streams.h"
#include "ace/ACE.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Reactor.h"
#include "ace/Proactor.h"
#include "ace/Singleton.h"
#include "ace/Atomic_Op.h"

#if (ACE_MAJOR_VERSION >= 5) && (ACE_MINOR_VERSION >= 4)
#include "ace/Trace.h"
#endif

#include "ace/Timer_Queue_T.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"

#define BCHAR   ACE_TCHAR
#define _TX     ACE_TEXT

#include "BEE_Export.h"
#include "beelib.h"

// wrapper macro fot log
#define ERROR_LOG(x)                  ACE_ERROR((LM_ERROR, ACE_TEXT(x)))
#define ERROR_LOG1(x,p1)              ACE_ERROR((LM_ERROR, ACE_TEXT(x),p1))
#define ERROR_LOG2(x,p1,p2)           ACE_ERROR((LM_ERROR, ACE_TEXT(x),p1,p2))
#define ERROR_LOG3(x,p1,p2,p3)        ACE_ERROR((LM_ERROR, ACE_TEXT(x),p1,p2,p3))
#define ERROR_LOG4(x,p1,p2,p3,p4)     ACE_ERROR((LM_ERROR, ACE_TEXT(x),p1,p2,p3,p4))
#define ERROR_LOG5(x,p1,p2,p3,p4,p5)  ACE_ERROR((LM_ERROR, ACE_TEXT(x),p1,p2,p3,p4,p5))

#define DEBUG_LOG(x)                  ACE_DEBUG((LM_DEBUG, ACE_TEXT(x)))
#define DEBUG_LOG1(x,p1)              ACE_DEBUG((LM_DEBUG, ACE_TEXT(x),p1))
#define DEBUG_LOG2(x,p1,p2)           ACE_DEBUG((LM_DEBUG, ACE_TEXT(x),p1,p2))
#define DEBUG_LOG3(x,p1,p2,p3)        ACE_DEBUG((LM_DEBUG, ACE_TEXT(x),p1,p2,p3))
#define DEBUG_LOG4(x,p1,p2,p3,p4)     ACE_DEBUG((LM_DEBUG, ACE_TEXT(x),p1,p2,p3,p4))
#define DEBUG_LOG5(x,p1,p2,p3,p4,p5)  ACE_DEBUG((LM_DEBUG, ACE_TEXT(x),p1,p2,p3,p4,p5))

// declare any types
typedef ACE_Module<ACE_MT_SYNCH>        MT_Module;
typedef ACE_Task<ACE_MT_SYNCH>          MT_Task;
typedef ACE_Message_Queue<ACE_MT_SYNCH> MT_Queue;

typedef ACE_Atomic_Op<ACE_Thread_Mutex, unsigned int>  MUTEX_UInt;
typedef ACE_Atomic_Op<ACE_Thread_Mutex, int>  MUTEX_Int;
typedef ACE_Atomic_Op<ACE_Thread_Mutex, bool> MUTEX_Bool;
typedef ACE_Atomic_Op<ACE_Thread_Mutex, void *> MUTEX_Ptr;

struct int_less
{
    bool operator() (int x, int y) const { return (x < y); }
};

struct uint_less
{
    bool operator() (UINT x, UINT y) const { return (x < y); }
};

struct tchar_less
{
   bool operator() (TCHAR * x, TCHAR * y) const { return _tcscmp(x, y) < 0; }
};

struct str_less
{
   bool operator() (string x, string y) const { return (x < y); }
};

struct cstr_less
{
   bool operator() (const string& x, const string& y) const { return (x < y); }
};

struct char_less
{
    bool operator() (char * x, char * y) const
            { return (strcmp(x, y) < 0); }
};

struct cchar_less
{
    bool operator() (const char * x, const char * y) const
            { return (strcmp(const_cast<char *>(x),
                             const_cast<char *>(y)) < 0); }
};

struct nocase_less
{
    bool operator() (string x, string y) const
    {
        string::iterator p = x.begin();
        string::iterator q = y.begin();
        while (p != x.end() && q != y.end() && toupper(*p) == toupper(*q))
        {
            ++p;
            ++q;
        }
        if (p == x.end())
            return (q != y.end());
        if (q == y.end())
            return false;
        return (toupper(*p) < toupper(*q));
    }
};

//
// macro for Mutex Lock
//
#define BEE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN,COUNT,MSEC)  \
    ACE_Guard< MUTEX > OBJ (LOCK);                          \
    int  ___##OBJ = COUNT;                                  \
    while (1) {                                             \
        if (OBJ.locked() != 0)  break;                      \
        ___##OBJ--;                                         \
        if (___##OBJ == 0) return RETURN;                   \
        ACE_Time_Value __tv##OBJ(0, MSEC * 1000);           \
        ACE_OS::sleep(__tv##OBJ);                           \
    }

#define BEE_GUARD_LOOP(MUTEX,OBJ,LOCK)                      \
    ACE_Guard< MUTEX > OBJ (LOCK);                          \
    while (1) {                                             \
        if (OBJ.locked() != 0)  break;                      \
        ACE_Time_Value __tv##OBJ(0, 250 * 1000);            \
        ACE_OS::sleep(__tv##OBJ);                           \
    }

//
// Timer Queue:
// These typedefs ensure that we use the minimal amount of locking necessary.
//
//#include "ace/Timer_Queue_Adapters.h"

typedef ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex> Upcall;
typedef ACE_Timer_Heap_T<ACE_Event_Handler *, Upcall, ACE_Null_Mutex>
                                                       Timer_Heap;
typedef ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, Upcall, ACE_Null_Mutex>
                                                       Timer_Heap_Iterator;
typedef ACE_Thread_Timer_Queue_Adapter<Timer_Heap>     Thread_Timer_Queue;

#endif /* B_ACE_H */
