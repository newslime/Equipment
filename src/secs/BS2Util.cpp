// $Id: BS2Util.cpp,v 1.7 2004/08/14 14:48:43 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Util.cpp
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

#include "beesecs.h"
#include "BS2TimeValue.h"

static int parseSFNum(const BCHAR * sfptr, int * sfnum);

//-----------------------------------------------------------------------------
//  Convert stream function number
//-----------------------------------------------------------------------------
int ParseStreamFunction(string& sfstr)
{
    int sfnum;
    int result = parseSFNum(sfstr.c_str(), &sfnum);
    if (sfnum < 0)
    {
        ACE_UNUSED_ARG(result);
        return -1;
    }
    return sfnum;
}

//-----------------------------------------------------------------------------
int parseSFNum(const BCHAR * sfptr, int * sfnum)
{
    int   snum, fnum;
    const BCHAR * stop;
    const BCHAR * ftop;

    BS2Assert(sfnum != NULL && sfptr != NULL);
    stop = ftop = sfptr + 1;
    while (*ftop != '\0' && *ftop != 'f' && *ftop != 'F')
        ftop++;
    if (*ftop == '\0')
    {
        b_printf(_TX("illegal stream function number %s"), sfptr);
        return -1;
    }
    ftop++;
    snum = _tcstol(stop, NULL, 10);
    fnum = _tcstol(ftop, NULL, 10);
    if (snum > 255 || fnum > 255)
    {
        b_printf(_TX("illegal stream function number %s"), sfptr);
        return -1;
    }
    *sfnum = ((snum << 8) + fnum);
    return 0;
}

//-----------------------------------------------------------------------------
//
int ascii2tm(const BCHAR * timestr, struct tm * outm)
{
    TRACE_FUNCTION(TRL_LOW, "ascii2tm");

    int   _month_offset;
    BCHAR _year[8];
    BCHAR _month[8];
    BCHAR _day[8];
    BCHAR _hour[8];
    BCHAR _minute[8];
    BCHAR _second[8];
    BCHAR _msec[8];
    int  timefmt = (_tcslen(timestr) == 12) ? 0 : 1;

    if (timefmt == 0)
    {   // 12 bytes format
        _year[0] = *(timestr + 0);
        _year[1] = *(timestr + 1);
        _year[2] = '\0';
        _month_offset = 2;
    }
    else
    {
        _year[0] = *(timestr + 0);
        _year[1] = *(timestr + 1);
        _year[2] = *(timestr + 2);
        _year[3] = *(timestr + 3);
        _year[4] = '\0';
        _month_offset = 4;
    }
    _month[0] = *(timestr + _month_offset);
    _month[1] = *(timestr + _month_offset + 1);
    _month[2] = '\0';
    _day[0] = *(timestr + _month_offset + 2);
    _day[1] = *(timestr + _month_offset + 3);
    _day[2] = '\0';
    _hour[0] = *(timestr + _month_offset + 4);
    _hour[1] = *(timestr + _month_offset + 5);
    _hour[2] = '\0';
    _minute[0] = *(timestr + _month_offset + 6);
    _minute[1] = *(timestr + _month_offset + 7);
    _minute[2] = '\0';
    _second[0] = *(timestr + _month_offset + 8);
    _second[1] = *(timestr + _month_offset + 9);
    _second[2] = '\0';
    _msec[0] = '\0';

    if (timefmt == 0)
    {   // 12 bytes format
        outm->tm_year = _ttoi(_year) + 100;   // the current year.
    }
    else
    {
        outm->tm_year = _ttoi(_year) - 1900;   // the current year.
        _msec[0] = *(timestr + _month_offset + 10);
        _msec[1] = *(timestr + _month_offset + 11);
        _msec[2] = '\0';
    }
    outm->tm_mon = _ttoi(_month) - 1;   // the current month; Jan=0, Feb=1, and so on.
    outm->tm_mday = _ttoi(_day);        // the current day of the month.
    outm->tm_hour = _ttoi(_hour);       // the current hour.
    outm->tm_min = _ttoi(_minute);      // the current minute.
    outm->tm_sec = _ttoi(_second);      // the current second.
    outm->tm_wday = 0;

    return BEE_SUCCESS;
}

//-----------------------------------------------------------------------------
//
int ascii2tv(const BCHAR * timestr, BS2TimeValue& outv)
{
    TRACE_FUNCTION(TRL_LOW, "ascii2tv");

    struct tm _time;
    ascii2tm(timestr, &_time);

    time_t t = mktime(&_time);

    ACE_Time_Value tv(t);
    outv = tv;
    return 0;
}

//-----------------------------------------------------------------------------
//
int ascii2tv(const BCHAR * timestr, ACE_Time_Value& outv)
{
    TRACE_FUNCTION(TRL_LOW, "ascii2tv");

    struct tm _time;
    ascii2tm(timestr, &_time);

    time_t t = mktime(&_time);

    outv = t;
    return 0;
}

//-----------------------------------------------------------------------------
//
void tv2string(BS2TimeValue& tv, string& retstr)
{
    TRACE_FUNCTION(TRL_LOW, "tv2string");

    BCHAR timebuf[128];
    time_t t = tv.sec();
    const BCHAR * format = _TX("%Y%m%d%H%M%S00");
    struct tm * _time = localtime(&t);
    _tcsftime(timebuf, sizeof(timebuf), format, _time);
    retstr = timebuf;
}

//-----------------------------------------------------------------------------
//
void tv2string(ACE_Time_Value& tv, string& retstr)
{
    TRACE_FUNCTION(TRL_LOW, "tv2string");

    BCHAR timebuf[128];
    time_t t = tv.sec();
    const BCHAR * format = _TX("%Y%m%d%H%M%S00");
    struct tm * _time = localtime(&t);
    _tcsftime(timebuf, sizeof(timebuf), format, _time);
    retstr = timebuf;
}

//-----------------------------------------------------------------------------
//
void tm2string(struct tm * _time, string& retstr)
{
    TRACE_FUNCTION(TRL_LOW, "tm2string");

    BCHAR timebuf[128];
    const BCHAR * format = _TX("%Y%m%d%H%M%S00");
    _tcsftime(timebuf, sizeof(timebuf), format, _time);
    retstr = timebuf;
}

//-----------------------------------------------------------------------------
//  Parse enumerate string
//-----------------------------------------------------------------------------
int parseEnumNames(const string& enames, vector<string>& retvec)
{
    TRACE_FUNCTION(TRL_CONSTRUCT, "parseEnumNames");

    string enumName;
    size_t head_pos = 0;
    size_t tail_pos = enames.find(',', head_pos);
    while (tail_pos != string::npos)
    {
        if ((tail_pos - head_pos) == 0)
        {
            enumName = _TX("");              // no token - "t1,,t3"
        }
        else
        {
            enumName = enames.substr(head_pos, tail_pos - head_pos);
        }
        retvec.push_back(enumName);
        head_pos = tail_pos + 1;
        tail_pos = enames.find(',', head_pos);
    }
    enumName = enames.substr(head_pos); // Get last token
    retvec.push_back(enumName);
    return (int)retvec.size();               // Return token count
}


