// $Id: BS2SECSReceiver.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2SECSReceiver.h
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

#ifndef BS2SECSRECEIVER_H
#define BS2SECSRECEIVER_H

#include "BS2Receiver.h"

class BS2Device;

//------------------------------------------------------------------------------
//
// SECS Message receiver
//
//------------------------------------------------------------------------------
class BEE_Export BS2SECSReceiver: public BS2Receiver
{

friend class BS2Sender;

public:
    BS2SECSReceiver(BS2Device * device) : BS2Receiver(device) 
	{ }

    virtual int  parse(BCHAR * data, int size);

    void T4(long t4) { m_T4.msec(t4); }
    long T4() const  { return (m_T4.msec()); }

private:
    ACE_Time_Value  m_T4;

};


#endif
