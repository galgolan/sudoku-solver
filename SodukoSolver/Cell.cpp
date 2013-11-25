#include "Cell.h"

#include <iostream>
#include <sstream>
using namespace std;

#define LOG_SUBJECT	"Cell"
#include "SudokuException.h"

void Cell::BuildCell(int val, int numPossibilities)
{
	m_numPossibilities = numPossibilities;
	m_val = val;

	if(m_numPossibilities <= 0)
		THROW("Cell initialized with an invalid number of possibilities!");

	if(m_val == 0)
		for(int i = 1; i <= m_numPossibilities; ++i)
			m_posVals.insert(i);
	else if ((m_val > 0) && (m_val <= m_numPossibilities))
		m_posVals.clear();
	else
		THROW("Cell initialized with an invalid value!");

	UpdateStatus();

	stringstream ss;
	ss << "Cell created/set - " << ToString();
	LOG(ss.str());
}

void Cell::operator = (const Cell& cell)
{
	m_val = cell.m_val;
	m_posVals = cell.m_posVals;
	m_numPossibilities = cell.m_numPossibilities;
	m_cellStatus = cell.m_cellStatus;
}

void Cell::AddPossibility (int pos)
{
	if((pos > 0) && (pos <= m_numPossibilities))
		m_posVals.insert(pos);
	else
		THROW("invalid possibility added to cell!");

	UpdateStatus();
}

bool Cell::DeletePossibility(int pos)
{
	TPossibilites::iterator it = m_posVals.find(pos);
	if(it != m_posVals.end())
	{
		m_posVals.erase(it);
		UpdateStatus();
		stringstream ss;
		ss << "Deleted possibility: " << pos << ' ' << ToString();
		LOG(ss.str());
	//	if(m_posVals.size() == 1)	//this is will fuck up constraints which have
	//		ChoosePossibility();	//loops to scan possibilities.
		return true;
	}
	return false;
}

int Cell::GetPossibilitie(unsigned int index) const
{
	if(index >= m_posVals.size())
		THROW("trying to access a possibilitie with invalid index!");

	TPossibilites::const_iterator itPos = m_posVals.begin();
	for(unsigned int i = 0; i < index; i++)
		++itPos;
	return *itPos;
}

ostream& operator << (ostream& os, const Cell& cell)
{
	os << cell.GetValue();
	return os;
}

string& Cell::ToString() const
{
	stringstream ss;
	ss << "Value: " << m_val << ", Possibilities: ";
	for(TPossibilites::const_iterator it = m_posVals.begin(); it != m_posVals.end(); ++it)
		ss << *it << ' ';
	ss << " (" << m_numPossibilities << ")";
	static string str;
	str = ss.str();
	return str;
}

void Cell::UpdateStatus()
{
	if(m_posVals.size() > 0)
		m_cellStatus = CELL_STATUS_UNSOLVED;
	else		//no possibilities, can be either solved or stuck
	{
		if(IsEmpty())
			m_cellStatus = CELL_STATUS_STUCK;
		else
			m_cellStatus = CELL_STATUS_SOLVED;			
	}
}