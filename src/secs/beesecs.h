// $Id: beesecs.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    beesecs.h
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

#ifndef BEESECS_H
#define BEESECS_H

#include "../bee/b_ace.h"
#include "../bee/beelib.h"
#include "semidef.h"

#ifndef BEE_LOCKED
#define BEE_LOCKED    (1)
#endif

#define BS2Assert BEEAssert
#define BS2Panic  BEEPanic

// Stream Function Number Table
typedef vector<int>    StreamFunctionNumbers;

extern int  parseSFNum(BCHAR * sfptr, int * sfnum);

#endif /* BEESECS_H */

