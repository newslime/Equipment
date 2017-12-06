
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

//namespace ResourceManager
class ResourceManager
{
public:
	// A structure retaining the path and file names separately.
	typedef struct _splittedPath
	{
        std::string path;
        std::string file;
    }splittedPath;

	static ResourceManager*	Get();
	static void				Free();

	ResourceManager();
	~ResourceManager();

    /**
     * Searches for zip files and adds them to PhysFS search path.
     */
    //void initialize();

    /**
     * Checks whether the given file or directory exists in the search path
     */
	bool fileExists(const std::string& filename);
    bool exists(const std::string& path, bool lookInSearchPath = true);

    /**
     * Returns the real file-system path of the resource with the given
     * resource path, or an empty string when no such resource exists.
     */
    std::string resolve(const std::string& path);

    /**
     * Allocates data into a buffer pointer for raw data loading. The
     * returned data is expected to be freed using <code>free()</code>.
     *
     * @param fileName The name of the file to be loaded.
     * @param fileSize The size of the file that was loaded.
     *
     * @return An allocated byte array containing the data that was loaded,
     *         or <code>NULL</code> on failure.
     * @note The array contains an extra \0 character at position fileSize.
     */
    char *loadFile(const std::string &fileName, int &fileSize);

    /**
     * Returns the filePath sub-part corresponding to the filename only.
     * @return splittedPath: the file path ending with '/' or '\'
     *                       and the file name alone.
     */
     splittedPath splitFileNameAndPath(const std::string &fullFilePath);
};

#endif
