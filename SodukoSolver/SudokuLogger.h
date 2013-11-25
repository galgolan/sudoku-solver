#ifndef _SUDOKULOGGER_H_INCLUDED_
#define _SUDOKULOGGER_H_INCLUDED_

#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#include <windows.h>

// to avoid re-defining this macro, define it before including this header.
#ifndef LOG_SUBJECT
	#define LOG_SUBJECT	"SudokuSolver"	//in case someone forgot to define it.
#endif

class SudokuLogger
{
public:
	~SudokuLogger() { CloseLog(); }
	static void CloseLog();
	static bool IsOpen() { return m_os->is_open(); }
	static const string& GetFilename() { return m_filename; }

	template<class T>
	static void WriteLogEntry(const string& LogSubject, const T& entry)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		if(m_os != NULL)
		{
			*m_os << m_pendingLogEntries.str();
			m_pendingLogEntries.clear();
			*m_os << "# " << time.wHour << ':' << time.wMinute << ':' << time.wSecond
				<< '.' << time.wMilliseconds << " <" << LogSubject << "> - " << entry
				<< endl;
		}
		else
		{
			//m_filename = "C:\\SudokuSolver.log";
			m_os = new ofstream(SudokuLogger::m_filename.c_str(), ios::out);
			WriteLogEntry(LogSubject, entry);
			//m_pendingLogEntries << "# " << time.wHour << ':' << time.wMinute << ':' 
			//	<< time.wSecond << '.' << time.wMilliseconds << " <" << LogSubject 
			//	<< "> - " << entry << endl;
		}
	}

private:
	static ofstream* m_os;
	static string m_filename;
	static stringstream m_pendingLogEntries;
};

#define LOG(x)	SudokuLogger::WriteLogEntry(LOG_SUBJECT, x)

#endif //_SUDOKULOGGER_H_INCLUDED_