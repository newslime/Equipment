// $Id: BS2ItemHeader.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ItemHeader.h
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

#ifndef BS2ITEMHEADR_H
#define BS2ITEMHEADR_H

#include "beesecs.h"

//-----------------------------------------------------------------------------
// ITEM HEADER
//-----------------------------------------------------------------------------
#if _MSC_VER >= 1100
#pragma pack(push)
#endif

struct BS2ItemHeader
{
    BYTE m_format;
    BYTE m_len[3];

    BS2ItemHeader() { }
    BS2ItemHeader(int format, int q, int length) { set(format, q, length); }
    BS2ItemHeader(int format, int length) 
	{
        int q;
        if (length < 256)
            q = 1;
        else if (length < 65536)
            q = 2;
        else if (length < 16777216)
            q = 3;
/*#if 0
        else
        {
            throw length_error(_TX("BS2ItemHeader::BS2ItemHeader"));
        }
#endif*/
        set(format, q, length);
	}
    
	int dataLength() const 
	{
        BYTE lenSize = m_format & 0x03;
        
		if (lenSize == 0)
		{
            //throw out_of_range(_TX("BS2ItemHeader::dataLength()"));
			return -1; //fix error by stone
		}
        
		return ((lenSize == 1) ? m_len[0] :
                (lenSize == 2) ? (m_len[0] << 8) + m_len[1] :
								 (m_len[0] << 16) + (m_len[1] << 8) + m_len[2]);
    }
    
	BYTE * data() const 
	{
        BYTE lenSize = m_format & 0x03;
        
		if (lenSize == 0)
            throw out_of_range(_TX("BS2ItemHeader::data()"));
        
		return ((lenSize == 1) ? (BYTE *)&m_len[1] :
                (lenSize == 2) ? (BYTE *)&m_len[2] :
                                 (BYTE *)&m_len[3]);
    }
    
	bool isList() const { return (((m_format >> 2) & 0x3F) == ATOM_LIST); }
    int  lenSize() const { return (m_format & 0x03); }
    int  format() const { return ((m_format >> 2) & 0x3F); }
    
	BYTE * set(int format, int lenSize, int len) 
	{
        if (lenSize == 0)
            throw out_of_range(_TX("BS2ItemHeader::set()"));

        m_format = ((format << 2) & 0xFC) + (lenSize & 0x03);
        
		if (lenSize == 1)
        {
            m_len[0] = len & 0xFF;
            return (BYTE *)&m_len[1];
        }
        else if (lenSize == 2)
        {
            m_len[0] = (len >> 8) & 0xFF;
            m_len[1] = len & 0xFF;
            return (BYTE *)&m_len[2];
        }
        else
        {
            m_len[0] = (len >> 16) & 0xFF;
            m_len[1] = (len >> 8) & 0xFF;
            m_len[2] = len & 0xFF;
            return (BYTE *)&m_len[3];
        }
	}
#ifdef __GNUC__
} __attribute__((packed));
#else
};
#endif

#if _MSC_VER >= 1100
#pragma pack(pop)
#endif


#endif /* BS2ITEMHEADER_H */
