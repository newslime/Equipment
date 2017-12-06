// $Id: b_time.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_time.cpp
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

#include "time.h"
#include "b_time.h"

//-----------------------------------------------------------------------------
// Calculate lapse seconds in the day. (static)
//-----------------------------------------------------------------------------
UINT b_time::lapse(time_t curtime)
{
    struct tm * newtime;
    struct tm begintime;
    time_t basetime;

    newtime = localtime(&curtime);            // Convert to local time.

    begintime.tm_mday  = newtime->tm_mday;    // Convert to begin of the day.
    begintime.tm_mon   = newtime->tm_mon;
    begintime.tm_year  = newtime->tm_year;
    begintime.tm_wday  = newtime->tm_wday;
    begintime.tm_yday  = newtime->tm_yday;
    begintime.tm_isdst = newtime->tm_isdst;
    begintime.tm_hour  = 0;
    begintime.tm_sec   = 0;
    begintime.tm_min   = 0;

    basetime = mktime(&begintime);

    double diffsec = difftime(curtime, basetime);
    UINT   lapse = (unsigned int)diffsec;

    return lapse;
}

//-----------------------------------------------------------------------------
// Make log file name by date. (static)
//-----------------------------------------------------------------------------
int b_time::makeDateName(time_t curtime, string& tvname)
{
    struct tm * newtime;
    BCHAR  buf[16];

    newtime = localtime(&curtime);            // Convert to local time.
#if 0
    // Use logging per date
    _stprintf(buf, _TX("%04d%02d%02d"), newtime->tm_year + 1900,
                                        newtime->tm_mon + 1,
                                        newtime->tm_mday);
#else
    _stprintf(buf, _TX("%d%02d%02d%02d%02d%02d"), newtime->tm_year + 1900,
                                                  newtime->tm_mon + 1,
                                                  newtime->tm_mday,
                                                  newtime->tm_hour,
                                                  newtime->tm_min,
                                                  newtime->tm_sec);
#endif
    tvname = buf;
    return 0;
}

