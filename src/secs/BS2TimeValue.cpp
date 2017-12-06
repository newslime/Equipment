// $Id: BS2TimeValue.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TimeValue.cpp
 *
 *  @author  Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2001-2004 BEE Co.,Ltd. All rights reserved.
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

#include "BS2TimeValue.h"

extern int  ascii2tv(const BCHAR * timestr, BS2TimeValue& outv);
extern void tv2string(BS2TimeValue& tv, string& retstr);

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
BS2TimeValue::BS2TimeValue(const string& ts)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TimeValue::BS2TimeValue");
    ascii2tv(ts.c_str(), *this);
}

//-----------------------------------------------------------------------------
// Get timestamp
//-----------------------------------------------------------------------------
void BS2TimeValue::get(string& ts)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TimeValue::get");
    tv2string(*this, ts);
}

//-----------------------------------------------------------------------------
// Set timestamp
//-----------------------------------------------------------------------------
void BS2TimeValue::set(const BCHAR * ts)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "BS2TimeValue::get");
    ascii2tv(ts, *this);
}
