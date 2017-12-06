
#include <cmath>
#include <map>
#include <set>
#include <fstream>
#include "configuration.h"
#include "../utils/logger.h"
#include "../tinyxml/tinyxml.h"
#include "../utils/utilsstring.h"

static std::map<std::string, std::string>	options;
static std::string							configPath;
static std::set<std::string>				processedFiles;

////////////////////////////////////
////////////Configuration///////////
////////////////////////////////////
static Configuration* s_Configuration_Instance = NULL;

Configuration* Configuration::Get()
{
	if (!s_Configuration_Instance)
		s_Configuration_Instance = new Configuration();
	
	return s_Configuration_Instance;
}

void Configuration::Free()
{
	delete s_Configuration_Instance;
	s_Configuration_Instance = NULL;
}

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
	deinitialize();
}

bool Configuration::initialize(const std::string& fileName)
{
     bool success;
	
	if (fileName.empty())
        configPath = "./xml/gemserver.xml";
    else
        configPath = fileName;

    //const bool success = readFile(configPath);
	success = readFile(configPath);

    //LOG_INFO("Using config file: " << configPath);

    return success;
}

void Configuration::deinitialize()
{
    processedFiles.clear();
}

bool Configuration::readFile(const std::string& fileName)
{
	std::string		key;
	std::string		value;
	
	TiXmlElement*	root;
	TiXmlElement*	node;
	TiXmlDocument*	doc = new TiXmlDocument();
	
	if( doc->LoadFile(fileName.c_str()) )
	{
		root = doc->FirstChildElement("configuration");
		
		if( root )
		{
			for (node = root->FirstChildElement(); node != NULL; node = node->NextSiblingElement())
			{	
				if ( strcmp(node->Value(), "option") == 0 )
				{
					if ( node->Attribute("name") && node->Attribute("value") )
					{
						key		= node->Attribute("name");
						value	= node->Attribute("value");

						if (!key.empty())
							options[key] = value;
					}
				}
			}//for (node = root->FirstChildElement(); node != NULL; node = node->NextSiblingElement())

			delete doc;
			return true;
		
		}//if( root )

	}//if( doc->LoadFile(fileName.c_str()) )

	delete doc;
	return false;
}

std::string Configuration::getValue(const std::string &key,
                                    const std::string &deflt)
{
    std::map<std::string, std::string>::iterator iter = options.find(key);
    if (iter == options.end())
        return deflt;
    return iter->second;
}

int Configuration::getValue(const std::string &key, int deflt)
{
    std::map<std::string, std::string>::iterator iter = options.find(key);
    if (iter == options.end())
        return deflt;
    return atoi(iter->second.c_str());
}

bool Configuration::getBoolValue(const std::string &key, bool deflt)
{
    std::map<std::string, std::string>::iterator iter = options.find(key);
    if (iter == options.end())
        return deflt;
    return utils::stringToBool(iter->second.c_str(), deflt);
}
