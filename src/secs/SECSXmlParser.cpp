// $Id: SECSXmlParser.cpp,v 1.7 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    SECSXmlParser.cpp
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

#include "BS2ItemType.h"
#include "BS2ItemDictionary.h"
#include "BS2Message.h"
#include "BS2MessageDictionary.h"
#include "../tinyxml/tinyxml.h"
#include "SECSXmlParser.h"

static SECSXmlParser * _instance   = NULL;

static const BCHAR * _TAG_secs      = _TX("secs");
static const BCHAR * _TAG_itemTypes = _TX("itemTypes");
static const BCHAR * _TAG_itemType  = _TX("itemType");
static const BCHAR * _TAG_format    = _TX("format");
static const BCHAR * _TAG_messages  = _TX("messages");
static const BCHAR * _TAG_message   = _TX("message");
static const BCHAR * _TAG_body      = _TX("body");
static const BCHAR * _TAG_list      = _TX("list");
static const BCHAR * _TAG_item      = _TX("item");

// Function prototype
static UINT _formatNameToBit(string& formatStr);
static int  _stringToBool(string& boolStr, bool& tf);
static UINT _multiToCode(string& multiStr);

extern int ParseStreamFunction(string& sfstr);

//-----------------------------------------------------------------------------
// Refer multi block to inquiring message
//-----------------------------------------------------------------------------
struct MessageReference
{
    UINT m_sfnum;
    BS2MessageType * m_inquire;
    vector<BS2MessageType *> m_refers;

    MessageReference() : m_sfnum(0), m_inquire(NULL) {}
    MessageReference(UINT target, BS2MessageType * ref) : m_sfnum(target) {
            m_inquire = NULL;
            m_refers.push_back(ref);
        }
    MessageReference(const MessageReference& rhs) {
            m_sfnum = rhs.m_sfnum;
            m_inquire = rhs.m_inquire;
            m_refers = rhs.m_refers;
        }
    MessageReference& operator=(MessageReference& rhs) {
            if (this == &rhs)
                return *this;
            m_sfnum = rhs.m_sfnum;
            m_inquire = rhs.m_inquire;
            m_refers = rhs.m_refers;
            return *this;
        }

};

// Multi-block message table
static MessageReference * _multi[MAX_STREAM];

//
static MessageReference * _findReference(UINT sf)
{
    return _multi[STREAMCODE(sf) & 0x7F];
}

//
static void _addReference(MessageReference& ref) 
{
   _multi[STREAMCODE(ref.m_sfnum) & 0x7F] = new MessageReference(ref);
}

//
static void _addReference(UINT isf, BS2MessageType * refmsg) 
{
    MessageReference * refptr;
    if (isf == SINGLE_BLOCK_MESSAGE || isf == GRANT_MESSAGE)
    {   // ignore
        return ;
    }
    if (isf == INQUIRE_MESSAGE)
    {   // detect inquire message
        refptr = _findReference(refmsg->sfnum());
        if (refptr == NULL)
        {
            MessageReference ref;
            ref.m_sfnum = refmsg->sfnum();
            ref.m_inquire = refmsg;
            _addReference(ref);
            return ;
        }
        refptr->m_sfnum = refmsg->sfnum();
        refptr->m_inquire = refmsg;
        return ;
    }
    else if (isf == MULTI_BLOCK_MESSAGE)
    {
        refptr = _findReference(refmsg->sfnum());
        if (refptr == NULL)
        {
            MessageReference ref;
            ref.m_sfnum = SFCODE(STREAMCODE(refmsg->sfnum()), 0);
            ref.m_inquire = NULL;
            ref.m_refers.push_back(refmsg);
            _addReference(ref);
            return ;
        }
        refptr->m_refers.push_back(refmsg);
        return ;
    }

    refptr = _findReference(isf);
    if (refptr == NULL)
    {
        MessageReference ref(isf, refmsg);
        _addReference(ref);
        return ;
    }
    refptr->m_refers.push_back(refmsg);
}

//
static void _solveReference() 
{
    BS2MessageType * inqmsg;
    for (int i = 0; i < MAX_STREAM; i++)
    {
        MessageReference * ref = _multi[i];
        if (ref == NULL)
            continue;
        if (ref->m_inquire == NULL)
        {   // Undefined inquire message
            // Display error message
            inqmsg = BS2MessageDictionary::instance()->search(ref->m_sfnum);
            if (inqmsg == NULL)
            {   // Illegal sfnum
                // Display error message
                continue;
            }
        }
        else
        {
            inqmsg = ref->m_inquire;
        }

        if (inqmsg->multi() != INQUIRE_MESSAGE)
        {   // Not inquiry message
            // Display error message
            inqmsg = NULL;
            continue;
        }
        else
        {   // Mult-Block grant message
            BS2MessageType * grant =
                BS2MessageDictionary::instance()->search(ref->m_sfnum + 1);
            if (grant != NULL)
            {
                grant->setGrant();
            }
            else
            {
                // Display undefine grant message error.
            }
        }

        for (size_t i = 0; i < ref->m_refers.size(); i++)
        {
            BS2MessageType * msg = ref->m_refers[i];
            BS2Assert(msg != NULL);
            // Set inquiring message in multi block message
            msg->inqNum(inqmsg->sfnum());
        }
    }
}

static void _eraseReference() {
    for (int i = 0; i < MAX_STREAM; i++)
    {
        MessageReference * ref = _multi[i];
        if (ref == NULL)
            continue;
        delete ref;
        _multi[i] = NULL;
    }
}

//-----------------------------------------------------------------------------
// Instance of own
//-----------------------------------------------------------------------------
SECSXmlParser * SECSXmlParser::instance()
{
    if (_instance == NULL)
    {
        _instance = new SECSXmlParser;
    }
    return _instance;
}

void SECSXmlParser::close_singleton(void)
{
    if (_instance)
    {
       delete _instance;
	   _instance = NULL;
    }
}

//-----------------------------------------------------------------------------
// Parse message definition
//-----------------------------------------------------------------------------
int SECSXmlParser::parseMessage(const char* xmlFile)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseMessage");
    int result;

    result = this->init(xmlFile);
    if (result < 0)
    {
        return result;
    }
    result = this->parse();
    if (result < 0)
    {
        this->fini();
        return result;
    }

    _solveReference();

    this->fini();

    return result;
}

//-----------------------------------------------------------------------------
// Initialize
//-----------------------------------------------------------------------------
int SECSXmlParser::init(const char* xmlFile)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::init");
    // Initial multi inquiry message table
    for (int i = 0; i < MAX_STREAM; i++)
    {
        _multi[i] = NULL;
    }

    m_xmlFile = (xmlFile == NULL) ? _TX("") : xmlFile;

    return 0;
}

//-----------------------------------------------------------------------------
// Finish
//-----------------------------------------------------------------------------
int SECSXmlParser::fini()
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::fini");

    _eraseReference();

    return 0;
}

//-----------------------------------------------------------------------------
// Parse xml file
//-----------------------------------------------------------------------------
int SECSXmlParser::parse()
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parse");

    int				result;
	TiXmlDocument*	doc = new TiXmlDocument();

	// Traverse xml tree
	if( doc->LoadFile(m_xmlFile.c_str()) )
	{
		result = parseElements(doc->FirstChildElement());
		if(result < 0)
		{
			delete doc;
			return result;
		}
	}
	else
	{
		 TRACE_ERROR((_TX("Parsed document is null.\n")));
	}

	delete doc;
    return 0;
}

//-----------------------------------------------------------------------------
// Parse element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseElements(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseElements");

    int result = 0;
    TiXmlElement* child;
    if (node == NULL)
    {
        TRACE_ERROR((_TX("Node is null.\n")));
        return -1;
    }

	string nameStr = node->Value();
    if (nameStr != _TAG_secs)
    {
        TRACE_ERROR((_TX("Illegal node name <%s>.\n"), nameStr.c_str()));
        return -1;
    }

    // Attributes: nothing
	int nodeType = node->Type();

    switch (nodeType)
    {
	case TiXmlNode::ELEMENT:
        {
            // Parse for the presence of children.
			for (child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
			{
				switch (child->Type())
				{
				case TiXmlNode::ELEMENT:
					{
						nameStr = child->Value();
						if (nameStr == _TAG_itemTypes)
						{
							result = parseItemTypesElement(child);
							if (result < 0)
							{
								return result;
							}
						}
						else if (nameStr == _TAG_messages)
						{
							result = parseMessagesElement(child);
							if (result < 0)
							{
								return result;
							}
						}
						else
						{
							TRACE_ERROR((_TX("Ignore element name <%s>.\n"),
										 nameStr.c_str()));
							//return -1;
						}
					}
					break;

				case TiXmlNode::DOCUMENT:
				//case TixmlNode::COMMENT:
				case TiXmlNode::TEXT:
				case TiXmlNode::DECLARATION:
				case TiXmlNode::TYPECOUNT:
					break;
				default:
                    break;
				}
			}
        }
        break;

	case TiXmlNode::DOCUMENT:
	//case TixmlNode::COMMENT:
	case TiXmlNode::TEXT:
	case TiXmlNode::DECLARATION:
	case TiXmlNode::TYPECOUNT:
        break;

    default:
        break;
    }

    // Resolve undefined reference
    for (size_t i = 0; i < m_itemRefers.size(); i++)
    {
        BS2ItemDictionary * itemTable = BS2ItemDictionary::instance();
        BS2ItemType * itemType;
        itemType = const_cast<BS2ItemType *>(itemTable->find(m_itemRefers[i].m_itemName));
        if (itemType == NULL)
        {
            TRACE_ERROR((_TX("The item was not found (%s).\n"),
                        m_itemRefers[i].m_itemName.c_str()));
            continue;
        }
        if (! itemType->isList())
        {

            TRACE_ERROR((_TX("The item was not list (%s).\n"),
                        m_itemRefers[i].m_itemName.c_str()));
            continue;
        }

        // Link items into list.
        BS2Itemtypes * objList = reinterpret_cast<BS2Itemtypes *>(itemType);
        for (size_t j = 0; j < m_itemRefers[i].m_refers.size(); j++)
        {   // Return item data
            BS2ItemType * refItem;
            refItem = itemTable->find(m_itemRefers[i].m_refers[j]);
            if (refItem == NULL)
            {
                TRACE_ERROR((_TX("The item was not found (%s).\n"),
                            m_itemRefers[i].m_refers[j].c_str()));
                continue;
            }
            objList->add(refItem);  // <list>  -----  objList
        }                           //   <list>
                                    //      <item>    (name)
                                    //      <item>    (value)
    }

    return result;
}

//-----------------------------------------------------------------------------
// Parse <itemTypes> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseItemTypesElement(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseItemTypesElement");

    int result;
    TiXmlElement* child;

    // Attributes: nothing

    // Parse for the presence of children.
    for (child = node->FirstChildElement(); child != NULL;
         child = child->NextSiblingElement())
    {
		switch (child->Type())
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr = child->Value();
                if (nameStr == _TAG_itemType)
                {
                    result = parseItemType(child);
                    if (result < 0)
                    {
                        return result;
                    }
                }
                else
                {
                    TRACE_ERROR((_TX("Invalid element name %s.\n"),
                                 nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
		//case TixmlNode::COMMENT:
		case TiXmlNode::TEXT:
		case TiXmlNode::DECLARATION:
		case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// Parse <itemType> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseItemType(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseItemType");

    int				result = 0;
    TiXmlElement*	child;
	TiXmlAttribute* attribute;
    string			itemName = _TX("");
    string			itemType = _TX("");
    UINT			defaultFormat;
    UINT			formats;
    RefItems		listMembers;

	for(attribute = node->FirstAttribute(); attribute != NULL; attribute = attribute->Next())
	{
		string nameStr	= attribute->Name();
		string valStr	= attribute->Value();

		if (nameStr == _TX("name"))
        {
            itemName = valStr;
        }
		else if (nameStr == _TX("type"))
        {
            defaultFormat = _formatNameToBit(valStr);
            if (defaultFormat == BS2ItemType::UNKNOWN_FORMAT)
            {
                TRACE_ERROR((_TX("ItemType(%s) is defined invalid default faormat (%s).\n"),
                             itemName.c_str(), valStr.c_str()));
                return -1;
            }
        }
        else
        {
            TRACE_ERROR((_TX("ItemType(%s) is defined invalid attribute (%s).\n"),
                         itemName.c_str(), nameStr.c_str()));
            return -1;
        }
	}

    formats = 0;                         // Erase format bit

    // Parse for the presence of children.
	for (child = node->FirstChildElement(); child != NULL;
		 child = child->NextSiblingElement())
    {
		int nodeType = child->Type();
        switch (nodeType)
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr =  child->Value();
                if (nameStr == _TAG_format)
                {
                    UINT fmtbit = parseFormat(child, listMembers);
                    if (fmtbit == 0)
                    {
                        return -1;
                    }
                    formats |= fmtbit;
                }
                else
                {
                    TRACE_ERROR((_TX("ItemType(%s) has invalid element name %s.\n"),
                                 itemName.c_str(), nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
		//case TixmlNode::COMMENT:
		case TiXmlNode::TEXT:
		case TiXmlNode::DECLARATION:
		case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }

    // ACE_DEBUG((LM_DEBUG, ACE_TEXT("itemType: %s.\n"), itemName.c_str()));


    if ((formats & BS2ItemType::LIST_BIT) && (listMembers.m_refers.size() > 0))
    {   // set itemType setObject true
        BS2ItemDictionary::instance()->add(itemName, formats, defaultFormat, true);
        listMembers.m_itemName = itemName;
        m_itemRefers.push_back(listMembers);
    }
    else
    {
        BS2ItemDictionary::instance()->add(itemName, formats, defaultFormat);
    }

    result = 0;
    return result;
}

//-----------------------------------------------------------------------------
// Parse <format> element
//-----------------------------------------------------------------------------
UINT SECSXmlParser::parseFormat(TiXmlElement* node, RefItems& listMembers)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseFormat");

    TiXmlElement*	child;
    string			formatStr;
    UINT			formatBit = 0;

	child = node->FirstChildElement();
	if(child == NULL)
	{
		formatStr = node->GetText();
		formatBit = _formatNameToBit(formatStr);
        if (formatBit == BS2ItemType::UNKNOWN_FORMAT && (formatStr.size() > 0))
        {
            TRACE_ERROR((_TX("Item format was not found (%s).\n"),
                         formatStr.c_str()));
            return 0;
        }
	}
	else
	{
		formatStr = node->GetText();
		formatBit = _formatNameToBit(formatStr);
        if (formatBit == BS2ItemType::UNKNOWN_FORMAT && (formatStr.size() > 0))
        {
            TRACE_ERROR((_TX("Item format was not found (%s).\n"),
                         formatStr.c_str()));
            return 0;
        }

		// Parse for the presence of children.
		for (child = node->FirstChildElement(); child != NULL;
			 child = child->NextSiblingElement())
		{
			switch (child->Type())
			{
			case TiXmlNode::ELEMENT:
				{
					string nameStr = child->Value();
					if (nameStr == "list")
					{
						int result = parseListName(child, listMembers);
						if (result < 0)
						{
							return result;
						}
					}
					else
					{
						TRACE_ERROR((_TX("Invalid element name <%s>.\n"),
									 nameStr.c_str()));
						// return -1;   // ignore
					}
				}
				break;

			case TiXmlNode::DOCUMENT:
			//case TixmlNode::COMMENT:
			case TiXmlNode::TEXT:
			case TiXmlNode::DECLARATION:
			case TiXmlNode::TYPECOUNT:
				break;
			default:
				break;
			}
		}
	}

    return formatBit;
}

//-----------------------------------------------------------------------------
// Parse <list> element in <format>
//-----------------------------------------------------------------------------
int SECSXmlParser::parseListName(TiXmlElement* node, RefItems& listMembers)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseListName");

    int				result = 0;
    TiXmlElement*	child;

    // Parse for the presence of children.
	for (child = node->FirstChildElement(); child != NULL;
		 child = child->NextSiblingElement())
    {
		int nodeType = child->Type();
        switch (nodeType)
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr =  child->Value();
                if (nameStr == "item")
                {
					string refName = child->GetText();
                    listMembers.m_refers.push_back(refName);
                }
                else
                {
                    TRACE_ERROR((_TX("Invalid element name <%s>.\n"),
                                 nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
		//case TixmlNode::COMMENT:
		case TiXmlNode::TEXT:
		case TiXmlNode::DECLARATION:
		case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
// Parse <messages> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseMessagesElement(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseMessagesElement");

    int				result;
    TiXmlElement*	child;

    // Attributes: nothing

    // Parse for the presence of children.
	for (child = node->FirstChildElement(); child != NULL;
		child = child->NextSiblingElement())
    {
		switch (child->Type())
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr = child->Value();
                if (nameStr == _TAG_message)
                {
                    result = parseMessage(child);
                    if (result < 0)
                    {
                        return result;
                    }
                }
                else
                {
                    TRACE_ERROR((_TX("Invalid element name <%s>.\n"),
                                 nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

       case TiXmlNode::DOCUMENT:
	   //case TixmlNode::COMMENT:
	   case TiXmlNode::TEXT:
	   case TiXmlNode::DECLARATION:
	   case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
// Parse <message> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseMessage(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseMessage");

    int				result		= 0;
    TiXmlElement*	child;
	TiXmlAttribute* attribute;
    string			mngrName	= _TX("");
    string			sfstr		= _TX("");
    UINT			sfnum;
    int				dir			= BOTH_DIR;
    bool			wait		= false;
    UINT			multi		= SINGLE_BLOCK_MESSAGE;

	for(attribute = node->FirstAttribute(); attribute != NULL; attribute = attribute->Next())
	{
		string nameStr	= attribute->Name();
		string valStr	= attribute->Value();

		if (nameStr == "sf")
		{
			sfnum = ParseStreamFunction(valStr);
            if ((sfnum & 0xFFFF0000) != 0)
            {
                TRACE_ERROR((_TX("StreamFunction is defined illegal stream-function number(%s).\n"),
                             valStr.c_str()));
                return -1;
            }
            sfstr = valStr;
            wait = ((sfnum & 1) == 1) ? true : false;
		}
		else if (nameStr == _TX("dir"))
		{
			if (valStr == _TX("BOTH"))
                dir = BOTH_DIR;
            else if (valStr == _TX("EtoH"))
                dir = EQUIPMENT_TO_HOST;
            else if (valStr == _TX("HtoE"))
                dir = HOST_TO_EQUIPMENT;
            else
            {
                TRACE_ERROR((_TX("Message(%s) is defined illegal direction(%s).\n"),
                             sfstr.c_str(), valStr.c_str()));
            }
		}
		else if (nameStr == _TX("reply"))
		{
			if (_stringToBool(valStr, wait) < 0)
            {
                TRACE_ERROR((_TX("Message(%s) is defined illegal reply:(%s).\n"),
                             sfstr.c_str(), valStr.c_str()));
                // return BEE_ERROR;  // ignore
            }
		}
		else if (nameStr == _TX("multi"))
		{
			multi = _multiToCode(valStr);
            if (multi == SINGLE_BLOCK_MESSAGE)
            {
                TRACE_ERROR((_TX("Message(%s) is defined illegal multi:(%s).\n"),
                             sfstr.c_str(), valStr.c_str()));
                // return BEE_ERROR;  // ignore
            }
		}
		else if (nameStr == _TX("category"))
		{
			mngrName = valStr;
		}
		else
		{
			TRACE_ERROR((_TX("Message(%s) is defined invalid attribute (%s).\n"),
                         sfstr.c_str(), nameStr.c_str()));
            return -1;
		}
	}

    BS2MessageType * msgtype;
    msgtype = new BS2MessageType(sfnum, dir, wait, mngrName, multi);

    _addReference(multi, msgtype);     // Entry Multi-Block reference

    // Parse for the presence of children.
	for (child = node->FirstChildElement(); child != NULL;
		 child = child->NextSiblingElement())
    {
		int nodeType = child->Type();
        switch (nodeType)
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr =  child->Value();
                if (nameStr == _TAG_body)
                {
                    result = parseBody(child, msgtype);
                    if (result < 0)
                    {
                        delete msgtype;
                        return result;
                    }
                }
                else
                {
                    TRACE_ERROR((_TX("Message(%s) has invalid element name <%s>.\n"),
                                 sfstr.c_str(), nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
	    //case TixmlNode::COMMENT:
	    case TiXmlNode::TEXT:
	    case TiXmlNode::DECLARATION:
	    case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }

    // ACE_DEBUG((LM_DEBUG, ACE_TEXT("message: %s.\n"), sfstr.c_str()));

    BS2MessageDictionary::instance()->add(msgtype);

    return result;
}

//-----------------------------------------------------------------------------
// Parse <body> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseBody(TiXmlElement* node, BS2MessageType * msgtype)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseBody");

    int				result = 0;
   TiXmlElement*	child;

    // Parse for the presence of children.
   for (child = node->FirstChildElement(); child != NULL;
	    child = child->NextSiblingElement())
    {
		int nodeType = child->Type();
        switch (nodeType)
        {
		case TiXmlNode::ELEMENT:
            {
				string nameStr =  child->Value();
                if (nameStr == _TAG_list)
                {
                    BS2Itemtypes * itemList = new BS2Itemtypes;
                    result = parseList(child, itemList);
                    if (result < 0)
                    {
                        return result;
                    }
                    msgtype->add(itemList);
                }
                else if (nameStr == _TAG_item)
                {
                    BS2ItemType * itemType = parseItem(child);
                    if (itemType == NULL)
                    {
                        return -1;
                    }
                    msgtype->add(itemType);
                }
                else
                {
                    TRACE_ERROR((_TX("Invalid element name <%s>.\n"),
                                 nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
	    //case TixmlNode::COMMENT:
	    case TiXmlNode::TEXT:
	    case TiXmlNode::DECLARATION:
	    case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
// Parse <list> element
//-----------------------------------------------------------------------------
int SECSXmlParser::parseList(TiXmlElement* node, BS2Itemtypes * items)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseList");

    int				result = 0;
    TiXmlElement*	child;

    // Parse for the presence of children.
	for (child = node->FirstChildElement(); child != NULL;
		 child = child->NextSiblingElement())
    {
		int nodeType = child->Type();
        switch (nodeType)
        {
        case TiXmlNode::ELEMENT:
            {
				string nameStr =  child->Value();
                if (nameStr == _TAG_list)
                {
                    BS2Itemtypes * itemList = new BS2Itemtypes;
                    result = parseList(child, itemList);
                    if (result < 0)
                    {
                        return result;
                    }
                    items->add(itemList);
                }
                else if (nameStr == _TAG_item)
                {
                    BS2ItemType * itemType = parseItem(child);
                    if (itemType == NULL)
                    {
                        return -1;
                    }
                    items->add(itemType);
                }
                else
                {
                    TRACE_ERROR((_TX("Invalid element name <%s>.\n"),
                                 nameStr.c_str()));
                    // return -1;   // ignore
                }
            }
            break;

        case TiXmlNode::DOCUMENT:
	    //case TixmlNode::COMMENT:
	    case TiXmlNode::TEXT:
	    case TiXmlNode::DECLARATION:
	    case TiXmlNode::TYPECOUNT:
            break;
        default:
            break;
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
// Parse <item> element
//-----------------------------------------------------------------------------
BS2ItemType * SECSXmlParser::parseItem(TiXmlElement* node)
{
    TRACE_FUNCTION(TRL_LOW, "SECSXmlParser::parseItem");

	string				itemStr;
    const BS2ItemType*	itemType;

	itemStr		= node->GetText();
    itemType	= BS2ItemDictionary::instance()->find(itemStr);

    if (itemType == NULL)
    {
        TRACE_ERROR((_TX("The item was not found (%s).\n"),
                     itemStr.c_str()));
        // return NULL;
    }
    else
    {   // Return item data
        return const_cast<BS2ItemType *>(itemType);
    }

    return NULL;
}


//----------------------------------------------------------------------------
// Convert format string to bit pattern
//----------------------------------------------------------------------------
struct
{
    BCHAR * m_fmtName;
    UINT   m_bit;
}
_format2bit[] =
{
    { _TX("list"),    BS2ItemType::LIST_BIT    },
    { _TX("binary"),  BS2ItemType::BINARY_BIT  },
    { _TX("boolean"), BS2ItemType::BOOLEAN_BIT },
    { _TX("ascii"),   BS2ItemType::ASCII_BIT   },
    { _TX("jis"),     BS2ItemType::JIS_BIT     },
    { _TX("unicode"), BS2ItemType::UNICODE_BIT },
    { _TX("intx"),    BS2ItemType::INT1_BIT | BS2ItemType::INT2_BIT |
                      BS2ItemType::INT4_BIT | BS2ItemType::INT8_BIT },
    { _TX("int1"),    BS2ItemType::INT1_BIT    },
    { _TX("int2"),    BS2ItemType::INT2_BIT    },
    { _TX("int4"),    BS2ItemType::INT4_BIT    },
    { _TX("int8"),    BS2ItemType::INT8_BIT    },
    { _TX("uintx"),   BS2ItemType::UINT1_BIT | BS2ItemType::UINT2_BIT |
                      BS2ItemType::UINT4_BIT | BS2ItemType::UINT8_BIT },
    { _TX("uint1"),   BS2ItemType::UINT1_BIT   },
    { _TX("uint2"),   BS2ItemType::UINT2_BIT   },
    { _TX("uint4"),   BS2ItemType::UINT4_BIT   },
    { _TX("uint8"),   BS2ItemType::UINT8_BIT   },
    { _TX("floatx"),  BS2ItemType::FLOAT4_BIT | BS2ItemType::FLOAT8_BIT },
    { _TX("float4"),  BS2ItemType::FLOAT4_BIT  },
    { _TX("float8"),  BS2ItemType::FLOAT8_BIT  },
    { NULL,           BS2ItemType::UNKNOWN_FORMAT }
};

// Convert string to bit pattern
UINT _formatNameToBit(string& formatStr)
{
    size_t delimit = formatStr.find_first_of(_TX(" \n"));
    if (delimit != formatStr.npos)
    {
        formatStr.resize(delimit);
    }
    int pos = 0;
    while (_format2bit[pos].m_fmtName != NULL)
    {
        if (formatStr == _format2bit[pos].m_fmtName)
        {
            break;
        }
        pos++;
    }
    return _format2bit[pos].m_bit;
}

//------------------------------------------------------------------------------
// Convert string to boolean
int _stringToBool(string& boolStr, bool& tf)
{
    int result = 0;
    if (boolStr == _TX("true") || boolStr == _TX("TRUE") ||
        boolStr == _TX("True"))
    {
        tf = true;
    }
    else if (boolStr == _TX("false") || boolStr == _TX("FALSE") ||
             boolStr == _TX("False"))
    {
        tf = false;
    }
    else
    {
        result = -1;
    }
    return result;
}

//------------------------------------------------------------------------------
// Convert multi-block attribute to code
UINT _multiToCode(string& multiStr)
{
    UINT result;
    if (multiStr == _TX("yes") || multiStr == _TX("YES") ||
        multiStr == _TX("Yes"))
    {
        result = MULTI_BLOCK_MESSAGE;
    }
    else if (multiStr == _TX("inquire") || multiStr == _TX("INQUIRE") ||
             multiStr == _TX("Inquire"))
    {
        result = INQUIRE_MESSAGE;
    }
    else if (multiStr == _TX("grant") || multiStr == _TX("GRANT") ||
             multiStr == _TX("Grant"))
    {
        result = GRANT_MESSAGE;
    }
    else
    {
        int sf = ParseStreamFunction(multiStr);
        result = (sf < 0) ? SINGLE_BLOCK_MESSAGE : sf;
    }
    return result;
}

