// $Id: SECSXmlParser.h,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    SECSXmlParser.h
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

#ifndef SECSXMLPARSER_H
#define SECSXMLPARSER_H

class BS2MessageType;
class BS2ItemType;
class BS2Itemtypes;
class TiXmlElement;

//-----------------------------------------------------------------------------
// Refer item name in list format.
//-----------------------------------------------------------------------------
struct RefItems
{
    string         m_itemName;
    vector<string> m_refers;

    RefItems() {}
    RefItems(const RefItems& rhs) {
            m_itemName = rhs.m_itemName;
            m_refers = rhs.m_refers;
        }
    RefItems& operator=(const RefItems& rhs) {
            if (this == &rhs)
                return *this;
            m_itemName = rhs.m_itemName;
            m_refers = rhs.m_refers;
            return *this;
        }
};


//-----------------------------------------------------------------------------
// XML parser class which defined SECS Message and Item.
//-----------------------------------------------------------------------------
class BEE_Export SECSXmlParser
{
public:
    SECSXmlParser(const BCHAR * xmlFile = NULL) {
            m_xmlFile = (xmlFile == NULL) ? "" : xmlFile;
        }
    ~SECSXmlParser() {}

    int parseMessage(const char* xmlFile = NULL);

    static SECSXmlParser* instance();
	static void close_singleton();

protected:
    int  parseElements(TiXmlElement* node);
    int  parseItemTypesElement(TiXmlElement* node);
    int  parseItemType(TiXmlElement* node);
    int  parseMessagesElement(TiXmlElement* node);
    UINT parseFormat(TiXmlElement* node, RefItems& listMembers);
    int  parseListName(TiXmlElement* node, RefItems& listMembers);
    int  parseMessage(TiXmlElement* node);
    int  parseBody(TiXmlElement* node, BS2MessageType * msgtype);
    int  parseList(TiXmlElement* node, BS2Itemtypes * items);
    BS2ItemType *  parseItem(TiXmlElement* node);
    int  init(const char* xmlFile = NULL);
    int  fini();
    int  parse();

// member variables
protected:
    string m_xmlFile;
    vector<RefItems> m_itemRefers;
};

#endif  // SECSXMLPARSER_H
