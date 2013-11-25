#ifndef _SUDOKUEXCEPTION_H_INCLUDED_
#define _SUDOKUEXCEPTION_H_INCLUDED_

#include <sstream>
#include <string>
using namespace std;

#include "SudokuLogger.h"

#define THROW(msg)	Throw(msg, __FILE__, __LINE__)

inline void Throw(const string& msg, const string& file, long line)
{
	stringstream prompt, logmsg;
	prompt << "\nException: " << msg << " thrown at: " << file 
		<< ", line: " << line << '.' << endl;
	logmsg << msg << " thrown at: " << file << ", line: " << line << '.' << endl;
	SudokuLogger::WriteLogEntry("Exception", logmsg.str());
	throw exception(prompt.str().c_str());
}

#endif //_SUDOKUEXCEPTION_H_INCLUDED_