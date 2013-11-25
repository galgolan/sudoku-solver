#include "CSPSolver.h"

#include <vector>
#include <map>
using namespace std;

ConstraintBase::TConstraints ConstraintBase::m_constraintVector;
IConstraint::Enroller<ConstraintI> ConstraintI::m_enroller;
IConstraint::Enroller<ConstraintII> ConstraintII::m_enroller;
IConstraint::Enroller<ConstraintIII> ConstraintIII::m_enroller;

ConstraintBase::~ConstraintBase()
{
	for(TConstraints::iterator it = IConstraint::m_constraintVector.begin();
			it != ConstraintBase::m_constraintVector.end(); ++it)
		delete *it;
}

void CSPSolver::RunConstraints(Board& board) const
{
	bool changed2 = false;
	do
	{
		changed2 = false;
		for(unsigned int i = 0; i < board.GetSize(); ++i)
		{
			Board::TCellBasket row = board.GetRow(i);
			Board::TCellBasket col = board.GetColumn(i);
			Board::TCellBasket region = board.GetRegion(i % board.GetRegionSize(), i / board.GetRegionSize());

			//run all constraints until there is no change
			bool changed;
			do
			{
				changed = false;
				ConstraintBase::TConstraints constraints = ConstraintBase::GetConstraints();
				for(unsigned int i = 0; i < constraints.size(); ++i)
				{
					changed |= constraints[i]->Run(row);
					changed |= constraints[i]->Run(col);
					changed |= constraints[i]->Run(region);
					changed2 |= changed;
				}

			} while((changed) && (board.GetStatus() == Board::BOARD_STATUS_UNSOLVED));
		}
	} while((changed2) && (board.GetStatus() == Board::BOARD_STATUS_UNSOLVED));
}

void CSPSolver::SolveRec(Board& board) const
{
	RunConstraints(board);
	if(board.GetStatus() != Board::BOARD_STATUS_UNSOLVED)
		return;	//the board is either stuck or solved, so there is nothing more we can do.	
	else
	{
		for(unsigned int x = 0; x < board.GetSize(); ++x)
			for(unsigned int y = 0; y < board.GetSize(); ++y)
				if(board[x][y].IsEmpty())
				{
					int pos = board[x][y].GetPossibilitie();
					Board newBoard(board);
					newBoard[x][y].ChoosePossibility();
					SolveRec(newBoard);
					if(newBoard.GetStatus() == Board::BOARD_STATUS_SOLVED)
					{
						board = newBoard;
						return;
					}
					else if(newBoard.GetStatus() == Board::BOARD_STATUS_STUCK)
					{
						board[x][y].DeletePossibility(pos);
						RunConstraints(board);
						x=0;
						y=0;
					}
					else if(newBoard.GetStatus() == Board::BOARD_STATUS_UNSOLVED)
						THROW("SolveRec screwed up!");
				}
	}
}

Board& CSPSolver::Solve()
{
	LOG("Starting to solve.");
	SolveRec(m_board);
	LOG("Board solved.");
	return m_board;
}

bool ConstraintI::Run(Board::TCellBasket& basket)
{
	bool changed = false;
	for(Board::TCellBasket::const_iterator itCell = basket.begin(); itCell != basket.end(); ++itCell)
		if((*itCell)->GetPossibilities().size() == 1)
		{
			(*itCell)->ChoosePossibility();
			changed = true;
		}

	return changed;
}

bool ConstraintII::Run(Board::TCellBasket& basket)
{
	set<int> imposVals;	//impossible values
	//scan for values and remember them as impossible for other cells.
	for(Board::TCellBasket::const_iterator itCell = basket.begin(); itCell != basket.end(); ++itCell)
		if(!(*itCell)->IsEmpty())
			imposVals.insert((*itCell)->GetValue());

	//remove the impossible values from other cells.
	bool changed = false;
	for(Board::TCellBasket::const_iterator it = basket.begin(); it != basket.end(); ++it)
		if((*it)->GetPossibilities().size() > 0)
			for(set<int>::const_iterator itPos = imposVals.begin(); itPos != imposVals.end(); ++itPos)
				changed |= (*it)->DeletePossibility(*itPos);

	return changed;
}

bool ConstraintIII::Run(Board::TCellBasket& basket)
{
	// map each cell with its possibilities
	multimap<int, Cell*> posMap;
	for(Board::TCellBasket::const_iterator itCell = basket.begin(); itCell != basket.end(); ++itCell)
		for(unsigned int i = 0; i < (*itCell)->GetPossibilities().size(); ++i)
			posMap.insert(pair<int, Cell*>((*itCell)->GetPossibilitie(i), *itCell));

	// reverse the first map. its like mapping each possibility to the cells which have it.
	multimap<set<Cell*>, set<int> > cellCountMap;
	for(multimap<int, Cell*>::const_iterator itPos = posMap.begin(); itPos != posMap.end(); ++itPos)
	{
		pair<multimap<int, Cell*>::const_iterator, multimap<int, Cell*>::const_iterator> pairIt;
		pairIt = posMap.equal_range(itPos->first);
		set<Cell*> cellSet;
		for(multimap<int, Cell*>::const_iterator it = pairIt.first; it != pairIt.second; ++it)
			cellSet.insert(it->second);
		
		multimap<set<Cell*>, set<int> >::iterator it = cellCountMap.find(cellSet);
		if(it == cellCountMap.end())
		{
			set<int> initSet;
			initSet.insert(itPos->first);
			cellCountMap.insert(pair<set<Cell*>, set<int> >(cellSet, initSet));
		}
		else
			it->second.insert(itPos->first);
	}

	// now search for N possibilities in N cells and delete the other possibilities (which are'nt in N).
	bool changed = false;
	for(multimap<set<Cell*>, set<int> >::iterator it = cellCountMap.begin(); it != cellCountMap.end(); ++it)
	{
		if(it->first.size() == it->second.size())	//if N possibilities where found in N cells....
			for(set<Cell*>::const_iterator itCell = it->first.begin(); itCell != it->first.end(); ++itCell)
				for(unsigned int i = 0; i < (*itCell)->GetPossibilities().size(); ++i)
				{
					int pos = (*itCell)->GetPossibilitie(i);
					if(it->second.find(pos) == it->second.end())
						changed |= (*itCell)->DeletePossibility(pos);
				}
	}

	return changed;
}