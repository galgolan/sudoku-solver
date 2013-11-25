#undef LOG_SUBJECT
#define LOG_SUBJECT "PerfCounter"
#include "SudokuException.h"

#include <sstream>
using namespace std;
#include "windows.h"

#include "PerfCounter.h"

PerfCounter g_perf;

//stack<PerfCounter::PerfEntry> PerfCounter::m_counterStack;
//vector<PerfCounter::PerfEntry> PerfCounter::m_results;
//unsigned int PerfCounter::m_timerResolution = PerfCounter::Init();

PerfCounter::PerfCounter(unsigned int targetResolutionInMs)
{
	//get the timer capabilities.
	TIMECAPS tc;
	if(timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
		THROW("PerfCounter failed to determine the timer's capabilities!");
	m_timerResolution = min(max(tc.wPeriodMin, targetResolutionInMs), tc.wPeriodMax);
	stringstream ss;
	ss << "Initialized with resolution of: " << m_timerResolution << "ms";
	//LOG(ss.str());
}
void PerfCounter::Pop()
{
	if(m_counterStack.size() == 0)
		THROW("PerfCounter::Pop() called on an empty stack!");

	PerfEntry entry = m_counterStack.top();
	m_counterStack.pop();

	stringstream ss;
	ss << entry.m_description << " took: " << timeGetTime() - entry.m_duration << "ms";
	timeEndPeriod(m_timerResolution);
	LOG(ss.str());
}
void PerfCounter::Push(const string& description)
{
	if(timeBeginPeriod(m_timerResolution) != TIMERR_NOERROR)
		THROW("PerfCounter failed to initialize the timer with the target resolution!");

	m_counterStack.push(PerfEntry(description, timeGetTime()));
}