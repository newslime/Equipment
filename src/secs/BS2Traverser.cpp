// $Id: BS2Traverser.cpp,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2Traverser.cpp
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

#include "BS2Traverser.h"
#include "BS2Message.h"
#include "BS2ListItem.h"

//-----------------------------------------------------------------------------
// constructor/destructor
//-----------------------------------------------------------------------------
//BS2Traverser::~BS2Traverser()
//{
//    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::~BS2Traverser");
//}

//-----------------------------------------------------------------------------
// Parse item
//-----------------------------------------------------------------------------
int BS2Traverser::parseItem(BS2Item *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::parseItem");

    return 0;
}

//-----------------------------------------------------------------------------
// Begin list
//-----------------------------------------------------------------------------
int BS2Traverser::beginList(BS2ListItem *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::beginList");
    if (m_nest >= 0)
    {
        m_nest++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Traverser::beginValueList(BS2Item *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::beginValueList");
    if (m_nest >= 0)
    {
        m_nest++;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// End list
//-----------------------------------------------------------------------------
int BS2Traverser::endList(BS2ListItem *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::endList");
    if (m_nest >= 0)
    {
        m_nest--;
    }
    return 0;
}

//-----------------------------------------------------------------------------
int BS2Traverser::endValueList(BS2Item *)
{
    TRACE_FUNCTION(TRL_LOW, "BS2Traverser::endValueList");
    if (m_nest >= 0)
    {
        m_nest--;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// dump
//-----------------------------------------------------------------------------
void BS2Traverser::dump() const
{
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d %d\n"), m_nest, m_errcode));
}
