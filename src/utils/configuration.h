
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

#ifndef DEFAULT_SERVER_PORT
	#define DEFAULT_SERVER_PORT 9601
#endif

//namespace Configuration
class Configuration
{
public:    
	static Configuration*	Get();
	static void				Free();
	
	Configuration();
	~Configuration();
	
	/**
     * Loads the configuration options into memory.
     *
     * @param filename path to the configuration file. When empty, the default
     *                 config file 'genserv.xml' is used.
     * @return whether the configuration file could be read
     */
    bool initialize(const std::string &fileName = std::string());

    void deinitialize();

	bool readFile(const std::string& fileName);
    
	/**
     * Gets an option as a string.
     * @param key option identifier.
     * @param deflt default value.
     */
    std::string getValue(const std::string &key, const std::string &deflt);

    /**
     * Gets an option as an integer.
     * @param key option identifier.
     * @param deflt default value.
     */
    int getValue(const std::string &key, int deflt);

    /**
     * Gets an option as a boolean.
     * @param key option identifier.
     * @param deflt default value.
     */
    bool getBoolValue(const std::string &key, bool deflt);
};

#endif // CONFIGURATION_H
