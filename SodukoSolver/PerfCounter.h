#ifndef _PERFCOUNTER_H_INCLUDED_
#define _PERFCOUNTER_H_INCLUDED_

#include <string>
#include <stack>
#include <vector>

class PerfCounter
{
public:
	class PerfEntry
	{
	public:
		PerfEntry(const string& desc, DWORD duration)
			: m_description(desc), m_duration(duration) {}

		string m_description;
		DWORD m_duration;
	};

	PerfCounter(unsigned int targetResolutionInMs = 1);
	void Pop();
	void Push(const string& description);

private:
	stack<PerfEntry> m_counterStack;
	vector<PerfEntry> m_results;
	unsigned int m_timerResolution;
};

extern PerfCounter g_perf;

#define PERF(desc, func)	g_perf.Push(desc);\
							func;\
							g_perf.Pop()

#define PERF_PUSH(desc)		g_perf.Push(desc)
#define PERF_POP			g_perf.Pop()

#endif //_PERFCOUNTER_H_INCLUDED_