/*
 *  The Mana Server
 *  Copyright (C) 2004-2010  The Mana World Development Team
 *  Copyright (C) 2010  The Mana Development Team
 *
 *  This file is part of The Mana Server.
 *
 *  The Mana Server is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana Server is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana Server.  If not, see <http://www.gnu.org/licenses/>.
 */

//#include "configuration.h"
#include "resourcemanager.h"
#include "utilsstring.h"
//#include "time.h"
#include "utilstime.h"

#include <fstream>
#include <iostream>

//#ifdef WIN32
//#include <windows.h>
//#endif

#include "logger.h"

using namespace utils;

//namespace utils
//{

/*static std::ofstream mLogFile;
std::string Logger::mFilename;
bool Logger::mHasTimestamp = true;
bool Logger::mTeeMode = false;
Logger::Level Logger::mVerbosity = Logger::Info;
bool Logger::mLogRotation = false;
long Logger::mMaxFileSize = 1024; // 1 Mb
bool Logger::mSwitchLogEachDay = false;
static std::string mLastCallDate;*/
/**
 * Old date
 * For code simplificatiion, the old Date is kept separate
 * from the last call date.
 */
//static std::string mOldDate;

/**
  * Check whether the day has changed since the last call.
  *
  * @return whether the day has changed.
  */
/*static bool getDayChanged()
{
    std::string dayDate = getCurrentDate();

    if (mLastCallDate != dayDate)
    {
        // Keep track of the old date.
        mOldDate = mLastCallDate;
        // Reset the current date for next call.
        mLastCallDate = dayDate;
        return true;
    }
    return false;
}*/

//////////////////////
///////Logger/////////
//////////////////////
static Logger* s_Logger_Instance = NULL;

Logger* Logger::Get()
{
	if (!s_Logger_Instance)
		s_Logger_Instance = new Logger();
	
	return s_Logger_Instance;
}

void Logger::Free()
{
	delete s_Logger_Instance;
	s_Logger_Instance = NULL;
}

Logger::Logger()
{
	mHasTimestamp		= true;
	mTeeMode			= false;
	mLogRotation		= false;
	mMaxFileSize		= 1024;
	mSwitchLogEachDay	= false;
	mVerbosity			= Logger::Info;
}

Logger::~Logger()
{
}

void Logger::initialize(const std::string &logFile)
{
    setLogFile(logFile, true);

    // Write the messages to both the screen and the log file.
	//setTeeMode(Configuration::Get()->getBoolValue("log_toStandardOutput", true));
    
	LOG_INFO("Using log file: " << logFile);

    // Set up the options related to log rotation.
	//setLogRotation(Configuration::Get()->getBoolValue("log_enableRotation", false));
    //setMaxLogfileSize(Configuration::Get()->getValue("log_maxFileSize", 1024));
    //setSwitchLogEachDay(Configuration::Get()->getBoolValue("log_perDay", false));
}

void Logger::output(std::ostream &os, const std::string &msg, const char *prefix)
{
    if (mHasTimestamp)
    {
        os << "[" << getCurrentTime() << "]" << ' ';
    }

    if (prefix)
    {
        os << prefix << ' ';
    }

    os << msg << std::endl;
}

void Logger::setLogFile(const std::string &logFile, bool append)
{
    // Close the current log file.
    if (mLogFile.is_open())
    {
        mLogFile.close();
    }

    // Open the file for output
    // and remove the former file contents depending on the append flag.
    mLogFile.open(logFile.c_str(),
                  append ? std::ios::app : std::ios::trunc);

    mFilename = logFile;
    mLastCallDate = mOldDate = getCurrentDate();

    if (!mLogFile.is_open())
    {
        throw std::ios::failure("unable to open " + logFile + "for writing");
    }
    else
    {
        // By default the streams do not throw any exception
        // let std::ios::failbit and std::ios::badbit throw exceptions.
        mLogFile.exceptions(std::ios::failbit | std::ios::badbit);
    }
}

void Logger::output(const std::string &msg, Level atVerbosity)
{
    static const char *prefixes[] =
    {
#ifdef T_COL_LOG
        "[\033[45mFTL\033[0m]",
        "[\033[41mERR\033[0m]",
        "[\033[43mWRN\033[0m]",
#else
        "[FTL]",
        "[ERR]",
        "[WRN]",
#endif
        "[INF]",
        "[DBG]"
		"[INS]"
    };

    if (mVerbosity >= atVerbosity)
    {
        bool open = mLogFile.is_open();

        if (open)
        {
            output(mLogFile, msg, prefixes[atVerbosity]);
            switchLogs();
        }

        if (!open || mTeeMode)
        {
            output(atVerbosity <= Warn ? std::cerr : std::cout,
                   msg, prefixes[atVerbosity]);
        }
    }
}

void Logger::switchLogs()
{
    int								fileNum;
	std::string						newFileName;
	ResourceManager::splittedPath	filePath;
	
	// Handles logswitch if enabled
    // and if at least one switch condition permits it.
    if (!mLogRotation || (mMaxFileSize <= 0 && !mSwitchLogEachDay))
        return;

    // Update current filesize
    long fileSize = mLogFile.tellp();

    bool dayJustChanged = getDayChanged();

    if ((fileSize >= (mMaxFileSize * 1024))
        || (mSwitchLogEachDay && dayJustChanged))
    {
        // Close logfile, rename it and open a new one
        mLogFile.flush();
        mLogFile.close();

        // Stringify the time, the format is: path/yyyy-mm-dd-n_logFilename.
        using namespace std;
        ostringstream os;
        os << (dayJustChanged ? mOldDate : getCurrentDate());

        fileNum = 1;
        
		//ResourceManager::splittedPath filePath = ResourceManager::splitFileNameAndPath(mFilename);
		filePath = ResourceManager::Get()->splitFileNameAndPath(mFilename);
       
        // Keeping a hard limit of 100 files per day.
        do
        {
            newFileName = filePath.path + os.str()
                                + "-" + toString<int>(fileNum)
                                + "_" + filePath.file;
        //}while (ResourceManager::exists(newFileName, false) && ++fileNum < 100);
		}while (ResourceManager::Get()->exists(newFileName, false) && ++fileNum < 100);

        if (rename(mFilename.c_str(), newFileName.c_str()) != 0)
        {
            // Continue appending on the original file.
            setLogFile(mFilename, true);
            mLogFile << "Error renaming file: " << mFilename << " to: "
            << newFileName << std::endl << "Keep logging on the same log file."
            << std::endl;
        }
        else
        {
            // Keep the logging after emptying the original log file.
            setLogFile(mFilename);
            mLogFile << "---- Continue logging from former file " << newFileName
                     << " ----" << std::endl;
        }
    }
}

/**
  * Check whether the day has changed since the last call.
  *
  * @return whether the day has changed.
  */
bool Logger::getDayChanged()
{
    std::string dayDate = getCurrentDate();

    if (mLastCallDate != dayDate)
    {
        // Keep track of the old date.
        mOldDate = mLastCallDate;
        // Reset the current date for next call.
        mLastCallDate = dayDate;
        return true;
    }
    return false;
}

//} // namespace utils
