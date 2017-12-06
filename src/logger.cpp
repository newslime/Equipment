
#include <fstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include "logger.h"


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
	char logFile[64];
	sprintf(logFile, "Equipmnt_%s.log", getCurrentDate().c_str());

	initialize(logFile);
}

Logger::~Logger()
{
	mLogFile.close();
}

void Logger::initialize(const string &logFile)
{
    setLogFile(logFile);
}

void Logger::write(const string &msg)
{
	if(getDayChanged())
	{
		char logFile[64];
		sprintf(logFile, "Equipmnt_%s.log", getCurrentDate().c_str());
		setLogFile(logFile);
	}

    mLogFile << msg << endl;
}

void Logger::setLogFile(const string &logFile)
{
    // Close the current log file.
    if (mLogFile.is_open())
    {
        mLogFile.close();
    }

    mLogFile.open(logFile.c_str(), ios::app);

    mFilename = logFile;

    if (!mLogFile.is_open())
    {
        throw ios::failure("unable to open " + logFile + "for writing");
    }
    else
    {
        mLogFile.exceptions(ios::failbit | ios::badbit);
    }
}

bool Logger::getDayChanged()
{
    string dayDate = getCurrentDate();

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

