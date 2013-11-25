#ifndef _BRUTEFORCESOLVER_H_INCLUDED
#define _BRUTEFORCESOLVER_H_INCLUDED

#include "SolverBase.h"
#undef LOG_SUBJECT
#define LOG_SUBJECT "BruteForceSolver"
#include "SudokuException.h"

class BruteForceSolver : public SolverBase
{
public:
	BruteForceSolver(const Board& board) : SolverBase(board)
		{ LOG("Created solver: " + Name()); }
	//BruteForceSolver(const BruteForceSolver& solver) {}
	~BruteForceSolver() {}

	virtual Board& Solve();
};

#endif //_BRUTEFORCESOLVER_H_INCLUDED