// $Id: BS2AnalyzeReceiver.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2AnalyzeReceiver.h
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

#ifndef BS2ANALYZERECEIVER_H
#define BS2ANALYZERECEIVER_H

#include "ace/Synch.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Containers.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue_Adapters.h"
#include "BS2Receiver.h"

class BS2Device;

//------------------------------------------------------------------------------
//
// HSMS Message receiver
//
//------------------------------------------------------------------------------
class BEE_Export BS2AnalyzeReceiver: public BS2Receiver
{
public:
    BS2AnalyzeReceiver(BS2Device * device) : BS2Receiver(device) 
	{ }

    virtual int dispatch(BCHAR * data, int size);
    virtual int parse(BCHAR * data, int size);

private:
};


#endif
