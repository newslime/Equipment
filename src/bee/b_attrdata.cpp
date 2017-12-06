// $Id: b_attrdata.cpp,v 1.6 2004/06/20 15:23:39 fukasawa Exp $

//=============================================================================
/**
 *  @file    b_attrdata.cpp
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

#include "b_attrdata.h"
#include "b_objtype.h"
#include "b_object.h"

//-----------------------------------------------------------------------------
// Constructor/Destructor
//-----------------------------------------------------------------------------
b_attrdata::b_attrdata(const b_object * object, const b_attribute * attr)
            : m_attr(const_cast<b_attribute *>(attr)),
              m_obj(const_cast<b_object *>(object))
{
    this->set(const_cast<b_value&>(attr->getInitVal()));
}

//-----------------------------------------------------------------------------
// Get object type
//-----------------------------------------------------------------------------
const b_objtype * b_attrdata::objtype() const
{
    TRACE_FUNCTION(TRL_LOW, "b_attribute::objtype");

    return m_obj->getObjType();
}

//-----------------------------------------------------------------------------
// Dump
//-----------------------------------------------------------------------------
void b_attrdata::dump() const
{
    string tmpbuf = this->toString();
    if (tmpbuf.size() > 512)
    {
        tmpbuf.resize(512);
        tmpbuf.append(_TX("\n...... cut characters ......\n"));
    }
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("    %d: %s = [%s]\n"),
                         this->m_attr->position(),
                         this->m_attr->name().c_str(),
                         tmpbuf.c_str()));
}

