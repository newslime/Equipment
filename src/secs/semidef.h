// $Id: semidef.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    semidef.h
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

#ifndef SEMIDEF_H
#define SEMIDEF_H

//
// SECS-I
//
const int SECS1_MIN_BLOCK = 0;
const int SECS1_MAX_BLOCK = 32767;
const int SECS1_MIN_TRANSACTION = 0;
const int SECS1_MAX_TRANSACTION = 65535;
const int SECS1_MIN_RETRY = 0;
const int SECS1_MAX_RETRY = 31;
const int SECS1_MIN_T1 = 100;
const int SECS1_MAX_T1 = 10000;
const int SECS1_MIN_T2 = 200;
const int SECS1_MAX_T2 = 25000;
const int SECS1_MIN_T3 = 1000;
const int SECS1_MAX_T3 = 120000;
const int SECS1_MIN_T4 = 1000;
const int SECS1_MAX_T4 = 120000;

const int SECS1_BLOCK_SIZE = 254;
const int HSMS_PORT_NUMBER = 5000;
const int MAX_STREAM = 128;

//
// SECS-II
//
//-----------------------------------------------------------------------------
// Item Format Code
#ifndef FORMAT_CODE_DEFINED
#define FORMAT_CODE_DEFINED
enum //octal
{
    ATOM_LIST = 000,
    ATOM_BINARY = 010,
    ATOM_BOOLEAN = 011,
    ATOM_ASCII = 020,
    ATOM_JIS = 021,
    ATOM_UNICODE = 022,
    ATOM_INT8 = 030,
    ATOM_INT1 = 031,
    ATOM_INT2 = 032,
    ATOM_INT4 = 034,
    ATOM_FLOAT8 = 040,
    ATOM_FLOAT4 = 044,
    ATOM_UINT8 = 050,
    ATOM_UINT1 = 051,
    ATOM_UINT2 = 052,
    ATOM_UINT4 = 054,
    ATOM_STRUCT = 070,
    ATOM_ARRAY = 0100,
    ATOM_VECTOR = 0170,
};
#endif

enum
{
    ALARM_SET = 0x80,                             // ALCD
    ALARM_ENABLED = 0x80,                         // ALED
};

enum
{
    CONTROL_EQUIPMENT_OFFLINE = 1,
    CONTROL_ATTEMPT_ONLINE = 2,
    CONTROL_HOST_OFFLINE = 3,
    CONTROL_ONLINE_LOCAL = 4,
    CONTROL_ONLINE_REMOTE = 5,
};

//
// GEM
//
const int MAX_LIMIT_ZONE = 7;
const int MAX_SLOT = 25;

#endif

