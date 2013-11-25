#ifndef _SOLVERBASE_H_INCLUDED_
#define _SOLVERBASE_H_INCLUDED_

#include "Board.h"

class SolverBase
{
public:
	SolverBase(const Board& board) : m_board(board) {}
	virtual ~SolverBase() {}

	virtual Board& Solve() = 0;

	// Returns: a string containing the name of the class 'this' points to.
	const string& Name() const
	{
		static string name = typeid(*this).name();
		return name;
	}

protected:
	Board m_board;
	//float m_precentPerCell;
};

#endif //_SOLVERBASE_H_INCLUDED_