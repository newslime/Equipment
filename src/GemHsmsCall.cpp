
#include <fstream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

#include "GemSDK.h"
#include "tinyxml/tinyxml.h"
#include "logger.h"

static GemHsmsCall* s_GemHsmsCall_Instance = NULL;

GemHsmsCall* GemHsmsCall::Get()
{
	if (!s_GemHsmsCall_Instance)
		s_GemHsmsCall_Instance = new GemHsmsCall();
	
	return s_GemHsmsCall_Instance;
}

void GemHsmsCall::Free()
{
	delete s_GemHsmsCall_Instance;
	s_GemHsmsCall_Instance = NULL;
}

GemHsmsCall::GemHsmsCall()
{
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	sprintf(m_ExePath, "%s", string( buffer ).substr( 0, pos).c_str());
}

GemHsmsCall::~GemHsmsCall()
{
}

void GemHsmsCall::Initialize(char* hostname, int port, int mode)
{
	DeviceParameter*	config;
	SocketParameter*	parm;

	ACE::init();
		
	config				= new SocketParameter();
	m_gemhsms_device	= new GemDevice(this);
	m_gemhsms_sender	= new GemSender(this, m_gemhsms_device);
	m_gemhsms_receiver	= new GemReceiver(this, m_gemhsms_device);
	
	parm = (SocketParameter*)config;
	parm->m_mode		= mode;
	parm->m_slave		= 0;
	parm->m_dtype		= DRIVER_SOCKET;
	parm->m_port		= port;

	if(mode == HSMS_MODE_ACTIVE)
		sprintf(parm->m_hostname, hostname);

	string filepath = string(m_ExePath) + "/xml/secs.xml";
	sprintf(parm->m_xmlname, filepath.c_str());

	m_gemhsms_device->initialize(parm, m_gemhsms_sender, m_gemhsms_receiver);
	m_gemhsms_device->open();
	
	delete config;

	InitCall();

	//parseSVIDList();
	//parseCEIDList();

	m_DataID			= 0;
	m_OnlineStatus		= false;
	m_CommunicateStatus = false;
}

void GemHsmsCall::Deinitialize()
{
	m_traceDataMap.clear();
	m_svidMap.clear();
	m_ceidMap.clear();
}

void GemHsmsCall::InitCall()
{
	int i, k;
		
	for(i=0; i<21; i++)
	{
		for(k=0;k<50;k++)
			m_hsmsrep[i][k] = NULL;
	}
			
	m_hsmsrep[1][0]	= &GemHsmsCall::HSMS_S1F0;
	m_hsmsrep[1][4]	= &GemHsmsCall::HSMS_S1F4;
	m_hsmsrep[1][13]= &GemHsmsCall::HSMS_S1F13;
	m_hsmsrep[1][14]= &GemHsmsCall::HSMS_S1F14;
	m_hsmsrep[1][16]= &GemHsmsCall::HSMS_S1F16;
	m_hsmsrep[1][18]= &GemHsmsCall::HSMS_S1F18;
	m_hsmsrep[2][24]= &GemHsmsCall::HSMS_S2F24;
	m_hsmsrep[2][34]= &GemHsmsCall::HSMS_S2F34;
	m_hsmsrep[2][36]= &GemHsmsCall::HSMS_S2F36;
	m_hsmsrep[2][38]= &GemHsmsCall::HSMS_S2F38;
}

/*void GemHsmsCall::SetGemSender(void* sender)
{
	m_gemhsms_sender = sender;
}*/

void GemHsmsCall::HsmsMessageAccess(int stnum, int fcnum, BS2MessageInfo* binfo)
{
	wxCriticalLocker lock(m_mutex_hsmscall);

	(this->*m_hsmsrep[stnum][fcnum])(binfo);
}

bool GemHsmsCall::isFunctionExist(int stnum, int fcnum)
{
	if( m_hsmsrep[stnum][fcnum] )
		return true;

	return false;
}

void GemHsmsCall::process()
{
	this->processS6F1();	
}

void GemHsmsCall::processS6F1()
{
	time_t								now;
	TraceData							traceData;
	map<string, TraceData>::iterator	itr;
	vector<string>						keys;
	
	if(!m_OnlineStatus)
	{ 
		m_traceDataMap.clear();
		return;
	}

	itr = m_traceDataMap.begin();
	for(int i = 0; i < m_traceDataMap.size(); i++)
	{
		traceData = itr->second;
		now = time(0);
		if((now - traceData.oritime) >= traceData.DSPER)
		{
			traceData.oritime = now;
			traceData.frequency++;
			this->HSMS_S6F1(traceData);
			
			m_traceDataMap[traceData.TRID] = traceData;
		}

		if(traceData.frequency >= traceData.TOTSMP)
		{
			keys.push_back(itr->first);
		}

		itr++;
	}

	//delete task
	for(int i = 0; i < keys.size(); i++)
	{
		m_traceDataMap.erase(keys[i]);
	}
}

void GemHsmsCall::setCommunicateStatus(BS2MessageInfo* info)
{
}

//------------------------------------------HSMS function------------------------------------------
void GemHsmsCall::HSMS_S1F0(BS2MessageInfo* info)
{
	BS2Message*	msg_body;
	msg_body = BS2Message::factory(SFCODE(1,0));

	msg_body->transNum(info->getMessage()->transNum());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S1F4(BS2MessageInfo* info)
{
	BS2ListItem*	rootlist;
	BS2Item*		item_value;
	BS2Message*		msg_body;
	string			svid;
	string			name;
	string			errStr = "";
	char			value[64];
	int				size;
	
	vector<b_value>				retval;
	vector<b_value>::iterator	itr;

	info->getMessage()->getValue("SVID", retval);

	size = retval.size();
	if(size > 0)
	{
		for(int i = 0; i < size; i++) 
		{
			svid = retval[i].toString();
			if(m_svidMap.find(svid) == m_svidMap.end())
			{
				errStr.append("SVID" + svid);
			}
		}

		if(errStr != "")
		{
			errStr.append("not exist");
			SendS9F7FormatError(info, errStr);
			return;
		}

		msg_body	= BS2Message::factory(SFCODE(1, 4));
		rootlist	= new BS2ListItem();
		itr			= retval.begin();

		for(int i = 0; i < size; i++)
		{
			sprintf(value, "%d", retval[i].toString().c_str());
			item_value = BS2Item::factory(_TX("SV"), new BS2Ascii(value));
			rootlist->add(item_value);

			itr++;
		}

		msg_body->transNum(info->getTransactionID());
		msg_body->add(rootlist);

		if( m_gemhsms_sender )
			m_gemhsms_sender->send(msg_body);

		delete msg_body;
	}
	else
	{
		SendS9F7FormatError(info, "S1F3:No SVID");
	}
}

void GemHsmsCall::HSMS_S1F13(BS2MessageInfo* info)
{
	BS2ListItem*	rootlist;
	BS2Item*		item_value;
	BS2Message*		msg_body;

	msg_body = BS2Message::factory(SFCODE(1, 13));
	rootlist = new BS2ListItem();

	item_value	= BS2Item::factory(_TX("MDLN"),new BS2Ascii("AlphaInfo"));
	rootlist->add(item_value);
	item_value	= BS2Item::factory(_TX("SVID"), new BS2Ascii("v1.0"));
	rootlist->add(item_value);

	msg_body->add(rootlist);

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;	
}	

void GemHsmsCall::HSMS_S1F14(BS2MessageInfo* info)
{
	BS2ListItem*	rootlist;
	BS2ListItem*	paramlist;
	BS2Item*		item_value;
	BS2Message*		msg_body;

	msg_body	= BS2Message::factory(SFCODE(1, 14));
	rootlist	= new BS2ListItem();
	paramlist	= new BS2ListItem();
			
	item_value	= BS2Item::factory(_TX("MDLN"), new BS2Ascii("AlphaInfo"));
	paramlist->add(item_value);

	item_value	= BS2Item::factory(_TX("SOFTREV"),new BS2Ascii("v1.0"));
	paramlist->add(item_value);

	item_value = BS2Item::factory(_TX("COMMACK"),new BS2Binary((byte)0x00));
	rootlist->add(item_value);
	rootlist->add(paramlist);

	msg_body->add(rootlist);
	msg_body ->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S1F16(BS2MessageInfo* info)
{
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body	= BS2Message::factory(SFCODE(1, 16));
	item_value	= BS2Item::factory(_TX("OFLACK"), new BS2Binary((byte)0x00));
	msg_body->add(item_value);
	msg_body ->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
	{
		m_gemhsms_sender->send(msg_body);
		m_OnlineStatus = false;
	}

	delete msg_body;
}

void GemHsmsCall::HSMS_S1F18(BS2MessageInfo* info)
{
	BS2Item*		item_value;
	BS2Message*		msg_body;

	msg_body	= BS2Message::factory(SFCODE(1, 18));

	if(m_OnlineStatus)
		item_value	= BS2Item::factory(_TX("ONLACK"), new BS2Binary((byte)0x02)); 
	else
		item_value	= BS2Item::factory(_TX("ONLACK"), new BS2Binary((byte)0x00)); 

	msg_body->add(item_value);
	msg_body ->transNum(info->getTransactionID());
	
	if( m_gemhsms_sender )
	{
		m_gemhsms_sender->send(msg_body);
		m_OnlineStatus = true;
	}

	delete msg_body;
}

void GemHsmsCall::HSMS_S2F24(BS2MessageInfo* info)
{
	/*
	0 - ok
	1 - too many SVIDs
	2 - no more traces allowed
	3 - invalid period
	4 - unknown SVID
	5 - bad REPGSZ
	6 - TRID redefinition
	*/

	int			TIAACK;
	TraceData	traceData;
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body = BS2Message::factory(SFCODE(2, 24));

	TIAACK = this->parseTraceData(info->getMessage(), traceData);

	if(TIAACK == 0)
		m_traceDataMap[traceData.TRID] = traceData;

	item_value = BS2Item::factory(_TX("TIAACK"), new BS2Binary((byte)TIAACK));
	msg_body->add(item_value);
	msg_body->transNum(info->getTransactionID());
	
	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S2F34(BS2MessageInfo* info)
{
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body = BS2Message::factory(SFCODE(2, 34));

	item_value = BS2Item::factory(_TX("DRACK"), new BS2Binary((byte)0x00));
	msg_body->add(item_value);
	msg_body->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S2F36(BS2MessageInfo* info)
{
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body = BS2Message::factory(SFCODE(2, 36));

	item_value = BS2Item::factory(_TX("LRACK"), new BS2Binary((byte)0x00));
	msg_body->add(item_value);
	msg_body->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S2F38(BS2MessageInfo* info)
{
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body = BS2Message::factory(SFCODE(2, 38));

	item_value = BS2Item::factory(_TX("ERACK"), new BS2Binary((byte)15));
	msg_body->add(item_value);
	msg_body->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S6F1(TraceData& traceData)
{
	BS2Item*		item_value;
	BS2Message*		msg_body;
	BS2ListItem*	rootlist;
	BS2ListItem*	ptlist;

	char		timebuf[80] = {0};
	struct tm	tstruct;
	time_t		now;
	int			size = traceData.SVIDList.size();

	msg_body	= BS2Message::factory(SFCODE(6, 1));
	rootlist	= new BS2ListItem();

	item_value = BS2Item::factory(_TX("TRID"), new BS2Ascii(traceData.TRID)); 
	rootlist->add(item_value);

	item_value = BS2Item::factory(_TX("SMPLN"), new BS2UInt4(traceData.frequency)); 
	rootlist->add(item_value);

	now = time(0);
	tstruct = *localtime(&now);
	strftime(timebuf, sizeof(timebuf), "%Y%m%d%H%M%S", &tstruct);

	item_value = BS2Item::factory(_TX("STIME"), new BS2Ascii(timebuf)); 
	rootlist->add(item_value);

	ptlist = new BS2ListItem();
	for(int i = 0; i < size; i++)
	{
		item_value = BS2Item::factory(_TX("SV"), new BS2UInt4(1)); 
		ptlist->add(item_value);
	}
	rootlist->add(ptlist);

	msg_body->add(rootlist);
	
	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;
}

void GemHsmsCall::HSMS_S6F11(string ceid)
{
	BS2Item*		item_value;
	BS2Message*		msg_body;
	BS2ListItem*	rootlist;
	BS2ListItem*	plist;
	BS2ListItem*	plist2;
	BS2ListItem*	plist3;

	msg_body	= BS2Message::factory(SFCODE(6, 11));
	rootlist	= new BS2ListItem();
	plist		= new BS2ListItem();
	plist2		= new BS2ListItem();
	plist3		= new BS2ListItem();

	item_value = BS2Item::factory(_TX("V"), new BS2Ascii("S6F11"));
	plist3->add(item_value);

	item_value = BS2Item::factory(_TX("RPTID"), new BS2UInt4(500));
	plist2->add(item_value);
	plist2->add(plist3);

	plist->add(plist2);

	item_value = BS2Item::factory(_TX("DATAID"), new BS2UInt4(m_DataID));
	rootlist->add(item_value);

	item_value = BS2Item::factory(_TX("CEID"), new BS2Ascii(ceid));
	rootlist->add(item_value);

	rootlist->add(plist);

	msg_body->add(rootlist);
	
	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;

	m_DataID++;
}

void GemHsmsCall::SendS9F7FormatError(BS2MessageInfo* info, string errStr)
{
	BS2Item*	item_value;
	BS2Message*	msg_body;

	msg_body	= BS2Message::factory(SFCODE(9, 7));
	item_value	= BS2Item::factory(_TX("ERRTEXT"), new BS2Ascii(errStr));
	msg_body->add(item_value);

	msg_body->transNum(info->getTransactionID());

	if( m_gemhsms_sender )
		m_gemhsms_sender->send(msg_body);

	delete msg_body;	
}

int GemHsmsCall::parseTraceData(BS2Message* msg, TraceData& traceData)
{
	/*
	0 - ok
	1 - too many SVIDs
	2 - no more traces allowed
	3 - invalid period
	4 - unknown SVID
	5 - bad REPGSZ
	6 - TRID redefinition
	*/

	string		trid;
	string		dsper;
	string		svid;
	int			totsmp;
	int			repgsz; 
	int			size;
	int			hour, minute, second;

	vector<b_value>				retval;
	vector<b_value>::iterator	svid_itr;

	trid	= msg->getItem("TRID")->atom()->toString();
	dsper	= msg->getItem("DSPER")->atom()->toString();
	totsmp	= msg->getItem("TOTSMP")->atom()->getInt();
	repgsz	= msg->getItem("REPGSZ")->atom()->getInt();

	if(totsmp == 0)
	{
		if(m_traceDataMap.find(trid) != m_traceDataMap.end())
		{
			m_traceDataMap.erase(trid);
			return 0;
		}
		else
		{
			return 6;
		}
	}

	if(m_traceDataMap.find(trid) != m_traceDataMap.end())
		return 6;

	if(dsper.length() == 6 && sscanf(dsper.c_str(),"%02d%02d%02d", &hour, &minute, &second) == 3)
	{
		traceData.DSPER = hour * 3600 + minute * 60 + second;
	}
	else
	{
		return 3;
	}

	if(totsmp < 0)
	{
		return 3;
	}
	else if(totsmp % repgsz != 0)
	{	
		return 5;
	}

	msg->getValue("SVID", retval);
	size = retval.size();

	if(size < 0)
		return 4;
	else if(size > 1000)
		return 1;

	svid_itr = retval.begin();
	for(int i = 0; i < size; i++)
	{
		svid = svid_itr->toString();
		if(m_svidMap.find(svid) == m_svidMap.end())
		{
			return 4;
			//traceData.SVIDList.push_back(svid);
		}

		svid_itr++;
	}

	traceData.TRID		= trid;
	traceData.TOTSMP	= totsmp;
	traceData.REPGSZ	= repgsz;
	traceData.SVIDList	= retval;
	traceData.oritime	= time(0);
	traceData.frequency	= 0;

	return 0;
}

int GemHsmsCall::parseSVIDList()
{
	//TRACE_FUNCTION(TRL_LOW, "GemHsmsCall::parseSVIDList");

	TiXmlDocument		doc;
	TiXmlElement*		node;
	TiXmlElement*		child;
	TiXmlAttribute*		attribute;
	string				nameStr;
	string				attribuName;
	string				attribuValue;
	string				svid;

	if(!doc.LoadFile("C:/AlphaProject/xml/SVIDNameList.xml"))
	{
		//TRACE_ERROR((_TX("Parsed document is null.\n")));
		return -1;
	}

	node = doc.FirstChildElement();

	switch (node->Type())
	{
		case TiXmlNode::ELEMENT:
        {
            // Parse for the presence of children.
			for (child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
			{
				nameStr = child->Value();
				svid = nameStr.substr(4, nameStr.size());
				switch (child->Type())
				{
				case TiXmlNode::ELEMENT:
					{
						for(attribute = child->FirstAttribute(); attribute != NULL; attribute = attribute->Next())
						{
							attribuName		= attribute->Name();
							attribuValue	= attribute->Value();
							if(attribuName == "NAME")
							{
								m_svidMap[svid] = attribuValue;
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

	return 0;
}

int GemHsmsCall::parseCEIDList()
{
	//TRACE_FUNCTION(TRL_LOW, "GemHsmsCall::parseSVIDList");

	TiXmlDocument		doc;
	TiXmlElement*		node;
	TiXmlElement*		child;
	TiXmlAttribute*		attribute;
	string				nameStr;
	string				attribuName;
	string				attribuValue;
	string				ceid;

	if(!doc.LoadFile("C:/AlphaProject/xml/CEIDNameList.xml"))
	{
		//TRACE_ERROR((_TX("Parsed document is null.\n")));
		return -1;
	}

	node = doc.FirstChildElement();

	switch (node->Type())
	{
		case TiXmlNode::ELEMENT:
        {
            // Parse for the presence of children.
			for (child = node->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
			{
				nameStr = child->Value();
				ceid = nameStr.substr(4, nameStr.size());
				switch (child->Type())
				{
				case TiXmlNode::ELEMENT:
					{
						for(attribute = child->FirstAttribute(); attribute != NULL; attribute = attribute->Next())
						{
							attribuName		= attribute->Name();
							attribuValue	= attribute->Value();
							if(attribuName == "NAME")
							{
								m_ceidMap[ceid] = attribuValue;
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

	return 0;
}