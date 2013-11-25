#include "SudokuLogger.h"

string SudokuLogger::m_filename = "C:\\SudokuSolver.log";
ofstream* SudokuLogger::m_os = new ofstream(SudokuLogger::m_filename.c_str(), ios::out);
stringstream SudokuLogger::m_pendingLogEntries;

void SudokuLogger::CloseLog()
{
	if(typeid(*m_os) == typeid(ofstream))
		((ofstream*)m_os)->close();
	delete m_os;
}