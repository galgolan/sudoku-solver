#ifndef _CELL_H_INCLUDED_
#define _CELL_H_INCLUDED_

#include <string>
#include <set>
using namespace std;

class Cell
{
public:
	typedef set<int> TPossibilites;

	enum ECellStatus
	{
		CELL_STATUS_UNSOLVED,
		CELL_STATUS_SOLVED,
		CELL_STATUS_STUCK
	};

	inline Cell(int val, int numPossibilities) { BuildCell(val, numPossibilities); }

	inline Cell(const Cell& cell) 
		: m_val(cell.m_val), m_posVals(cell.m_posVals),
		m_numPossibilities(cell.m_numPossibilities),
		m_cellStatus(cell.m_cellStatus)
	{}

	inline Cell(int numPossibilities) { BuildCell(0, numPossibilities); }

	//NOTE:	This operator only compares the value in the cells, 
	//		and not the possibilities of the cell.
	inline bool operator == (const Cell& cell) const { return (m_val == cell.m_val); }

	// This operator copies the value of the cell and the possibilities.
	void operator = (const Cell& cell);

	// NOTE: This operator uses the cell's previously defined m_numPossibilities.
	inline void operator = (int val) { BuildCell(val, m_numPossibilities); }

	void UpdateStatus();
	inline ECellStatus GetStatus() const {return m_cellStatus; }

	inline void SetValue(int val) { BuildCell(val, m_numPossibilities); }

	inline int GetValue() const { return m_val; }
	inline bool IsEmpty() const { return (m_val == 0); }

	// Sets the Nth possibilitie in the set as the cells value.
	inline void ChoosePossibility(unsigned int N = 0) { BuildCell(GetPossibilitie(N), m_numPossibilities); }
	inline const TPossibilites& GetPossibilities() const { return m_posVals; }
	int GetPossibilitie(unsigned int index = 0) const;
	void AddPossibility(int pos);

	//Returns:	False	-	If pos wasn't a possibilities of the cell,
	//			True	-	If pos was a possibilitie and was deleted.
	bool DeletePossibility(int pos);

	//Returns: A nice string of the format:
	//		"Value: <m_val>, Possibilities: <m_posVals> (<m_numPossibilities>)"
	string& ToString() const;

private:
	TPossibilites m_posVals;
	int m_val;
	int m_numPossibilities;
	ECellStatus m_cellStatus;

	// Creates a cell with val as the value.
	// when val != 0	m_posVals is filled with the numbers 0 - numPossibilities,
	// when val == 0	m_posVals is empty.
	void BuildCell(int val, int numPossibilities);
};

ostream& operator << (ostream& os, const Cell& cell);

#endif //_CELL_H_INCLUDED_