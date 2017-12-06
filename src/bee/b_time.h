// $Id: b_time.h,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_time.h
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

#ifndef B_TIME_H
#define B_TIME_H

#include "b_ace.h"

//-----------------------------------------------------------------------------
// bee time class.
//-----------------------------------------------------------------------------
class b_time
{
public:
    b_time() { memset(m_tv, 0, sizeof(m_tv)); }
    b_time(const struct tm& tv)      { set(tv); }
    b_time(const ACE_Time_Value& tv) { set(tv); }
    b_time(const b_time& rhs)        { copy(rhs); }
    ~b_time() {}

    b_time operator=(const b_time& rhs) {
            if (this == &rhs)
                return *this;
            copy(rhs);
            return *this;
        }
    b_time operator=(const string& rhs) {
            BCHAR * fmt = _TX("%02d%02d/%02d/%02d %02d:%02d:%02d.%d0");
            if (rhs.size() == 16) {
                fmt = _TX("%02d%02d%02d%02d%02d%02d%02d%02d");
            } else if (rhs.size() == 12) {
                m_tv[0] = m_tv[7] = 0;
                fmt = _TX("%02d%02d%02d%02d%02d%02d");
            }
            _stscanf(rhs.c_str(), fmt, &m_tv[0], &m_tv[1], &m_tv[2],
                                       &m_tv[3], &m_tv[4], &m_tv[5],
                                       &m_tv[6], &m_tv[7]);
            return *this;
        }

    ACE_Time_Value value() {
            struct tm tv;
            tv.tm_year = ((m_tv[0] * 100) + m_tv[1]) - 1900;
            tv.tm_mon = m_tv[2] - 1;
            tv.tm_mday = m_tv[3];
            tv.tm_hour = m_tv[4];
            tv.tm_min = m_tv[5];
            tv.tm_sec = m_tv[6];
            time_t t = mktime(&tv);
            ACE_Time_Value v(t, m_tv[7] * 10000);
            return v;
        }

    void toSemi(string& retstr) {
            BCHAR buf[48];
            _stprintf(buf, _TX("%02d%02d%02d%02d%02d%02d%02d"),
                           ((m_tv[0] * 100) + m_tv[1]), m_tv[2], m_tv[3], m_tv[4],
                           m_tv[5], m_tv[6], m_tv[7]);
            retstr = buf;
        }
    string toSemi() {
            string timeStr;
            toSemi(timeStr);
            return timeStr;     // As area is in heap, return value;
        }

    void toString(string& retstr) {
            BCHAR buf[48];
            _stprintf(buf, _TX("%d/%02d/%02d %02d:%02d:%02d.%02d"),
                           ((m_tv[0] * 100) + m_tv[1]), m_tv[2], m_tv[3], m_tv[4],
                           m_tv[5], m_tv[6], m_tv[7]);
            retstr = buf;
        }
    string toString() {
            string timeStr;
            toString(timeStr);
            return timeStr;     // As area is in heap, return value;
        }

    void set(const struct tm& tv) {
            m_tv[0] = (tv.tm_year + 1900) / 100;
            m_tv[1] = (tv.tm_year + 1900) % 100;
            m_tv[2] = tv.tm_mon + 1;
            m_tv[3] = tv.tm_mday;
            m_tv[4] = tv.tm_hour;
            m_tv[5] = tv.tm_min;
            m_tv[6] = tv.tm_sec;
            m_tv[7] = 0;
        }

    void set(const ACE_Time_Value& tv) {
            time_t sec = tv.sec();
            struct tm * when = localtime(&sec);
            set(*when);
            m_tv[7] = (BYTE)(tv.usec() / 10000);   // convert to 10msec
        }

    void set(const time_t& tv) {
            struct tm * when = localtime(&tv);
            set(*when);
            m_tv[7] = 0;
        }

    friend bool operator==(const b_time& lhs, const b_time& rhs);
    friend bool operator<(const b_time& lhs, const b_time& rhs);

    static UINT lapse(time_t curtime);
    static int  makeDateName(time_t curtime, string& tvname);

    void copy(const b_time& rhs) {
            m_tv[0] = rhs.m_tv[0]; m_tv[1] = rhs.m_tv[1];
            m_tv[2] = rhs.m_tv[2]; m_tv[3] = rhs.m_tv[3];
            m_tv[4] = rhs.m_tv[4]; m_tv[5] = rhs.m_tv[5];
            m_tv[6] = rhs.m_tv[6]; m_tv[7] = rhs.m_tv[7];
        }
//
public:
    BYTE m_tv[8];     // 0-1: year, 2:month, 3:day, 4:hour, 5:minutes, 6:second
                      // 7:10msec
};

// Operator function
inline bool operator==(const b_time& lhs, const b_time& rhs)
{
    if ((lhs.m_tv[0] == rhs.m_tv[0]) || (lhs.m_tv[1] == rhs.m_tv[1]) ||
        (lhs.m_tv[2] == rhs.m_tv[2]) || (lhs.m_tv[3] == rhs.m_tv[3]) ||
        (lhs.m_tv[4] == rhs.m_tv[4]) || (lhs.m_tv[5] == rhs.m_tv[5]) ||
        (lhs.m_tv[6] == rhs.m_tv[6]))
    {
        return true;
    }
    return false;
}

inline bool operator<(const b_time& lhs, const b_time& rhs)
{
    u_int lt = (lhs.m_tv[0] << 24) + (lhs.m_tv[1] << 16) +
               (lhs.m_tv[2] << 8) + lhs.m_tv[3];
    u_int rt = (rhs.m_tv[0] << 24) + (rhs.m_tv[1] << 16) +
               (rhs.m_tv[2] << 8) + rhs.m_tv[3];
    if (lt < rt)
        return true;
    else if (lt > rt)
        return false;

    lt = (lhs.m_tv[4] << 16) + (lhs.m_tv[5] << 8) + lhs.m_tv[6];
    rt = (rhs.m_tv[4] << 16) + (rhs.m_tv[5] << 8) + rhs.m_tv[6];
    if (lt < rt)
        return true;
    else if (lt > rt)
        return false;

    return false;
}

#endif  // B_TIME_H
