// $Id: BS2Traverser.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Traverser.h
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

#ifndef BS2TRAVERSER_H
#define BS2TRAVERSER_H

#include "beesecs.h"

class BS2Item;
class BS2ListItem;

//-----------------------------------------------------------------------------
// Parser for SECS Message
//-----------------------------------------------------------------------------
class BEE_Export BS2Traverser
{
public:
    BS2Traverser() : m_nest(0), m_errcode(0) {}
    virtual ~BS2Traverser() {}

    void dump() const;

    virtual int parseItem(BS2Item * item);
    virtual int beginList(BS2ListItem * listitem);
    virtual int endList(BS2ListItem * listitem);
    virtual int beginValueList(BS2Item * listitem);
    virtual int endValueList(BS2Item * listitem);

    int  nest()    { return m_nest; }
    int  errcode() { return m_errcode; }

//
protected:
    int  m_nest;
    int  m_errcode;
};


#endif
