
#ifndef LOGGER_H
#define LOGGER_H

#include <iosfwd>
#include <sstream>

using namespace std;

class Logger
{
public:
	static string getCurrentDate()
	{
		time_t now;
		tm local;

		// Get current time_t value
		time(&now);

		// Convert time_t to tm struct to break the time into individual
		// constituents.
		local = *(localtime(&now));

		// Stringify the time, the format is: yyyy-mm-dd
		using namespace std;
		ostringstream os;
		os << setw(4) << setfill('0') << (local.tm_year + 1900)
		   /*<< "-"*/ << setw(2) << setfill('0') << local.tm_mon + 1
		   /*<< "-"*/ << setw(2) << setfill('0') << local.tm_mday;

		return os.str();
	}

	static Logger*	Get();
	static void		Free();

	Logger();
	~Logger();

	void initialize(const string &logFile);
	void setLogFile(const string &logFile);
	void write(const string &msg);

private:
	bool getDayChanged();
	
	std::string		mFilename; /**< Name of the current logfile. */
	std::string		mLastCallDate;
	std::ofstream	mLogFile;
	std::string		mOldDate;

};



#endif // LOGGER_H
