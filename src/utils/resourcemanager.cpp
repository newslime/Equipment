
#include <fstream>
#include "resourcemanager.h"

#include "configuration.h"

#include "../utils/logger.h"

#include <sys/stat.h>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

static ResourceManager* s_ResourceManager_Instance = NULL;

ResourceManager* ResourceManager::Get()
{
	if (!s_ResourceManager_Instance)
		s_ResourceManager_Instance = new ResourceManager();
	
	return s_ResourceManager_Instance;
}

void ResourceManager::Free()
{
	delete s_ResourceManager_Instance;
	s_ResourceManager_Instance = NULL;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

/**
 * This function tries to check if a file exists based on the existence of
 * stats about it. Because simply trying to open it and check for failure is a
 * bad thing, as you don't know if you weren't able to open it because it
 * doesn't exist or because you don't have the right to.
 */
//static bool fileExists(const std::string &filename)
bool ResourceManager::fileExists(const std::string& filename)
{
    struct stat buffer;
    // When stat is succesful, the file exists
    return stat(filename.c_str(), &buffer) == 0;
}

bool ResourceManager::exists(const std::string& path, bool lookInSearchPath)
{
    //if (!lookInSearchPath)
	return fileExists(path);

    //return PHYSFS_exists(path.c_str());
}

ResourceManager::splittedPath ResourceManager::splitFileNameAndPath(const std::string &fullFilePath)
{
    // We'll reversed-search for '/' or'\' and extract the substrings
    // corresponding to the filename and the path separately.
    size_t slashPos = fullFilePath.find_last_of("/\\");

    ResourceManager::splittedPath splittedFilePath;
    // Note the last slash is kept in the path name.
    splittedFilePath.path = fullFilePath.substr(0, slashPos + 1);
    splittedFilePath.file = fullFilePath.substr(slashPos + 1);

    return splittedFilePath;
}
