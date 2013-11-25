#ifndef _CSPSolver_H_INCLUDED_
#define _CSPSolver_H_INCLUDED_

#include <set>
#include <vector>
#include <iostream>
using namespace std;

#include "SolverBase.h"
#undef LOG_SUBJECT
#define LOG_SUBJECT	"CSPSolver"
#include "SudokuException.h"

class ConstraintBase
{
public:
	typedef vector<ConstraintBase*> TConstraints;
	~ConstraintBase();

	// Should return true if a change was made to one of the cells.
	virtual bool Run(Board::TCellBasket& basket) = 0;

	// Returnes a vector containing one instance of each derived constraint.
	static const TConstraints& GetConstraints()
		{ return ConstraintBase::m_constraintVector; }

private:
	static TConstraints m_constraintVector;
	template<class TConstraint> friend struct Enroller;

protected:
	// Every constraint derived from IConstraint should have an Enroller
	// as a private static member, templated to the derived constraint's type.
	template<class TConstraint>
	struct Enroller
	{
		Enroller()
		{
			ConstraintBase::m_constraintVector.push_back(new TConstraint);
			LOG(string("Registered constraint: ") + string(typeid(TConstraint).name()));
		}
	};
};

// hides GetConstraints() in derived classes.
class IConstraint : public ConstraintBase
{
	using ConstraintBase::GetConstraints;
};

//I:	A cell's with only one possibilitie should have it as a value.
class ConstraintI : public IConstraint
{
public:
	virtual bool Run(Board::TCellBasket& basket);
private:
	static Enroller<ConstraintI> m_enroller;
};

//II:	A cell's value should not be a possibility of any other cell.
class ConstraintII : public IConstraint
{
public:
	virtual bool Run(Board::TCellBasket& basket);
private:
	static Enroller<ConstraintII> m_enroller;
};

//III:	N possibilities from a group of N different cells, which are not possibilities
//		of any other cells, should be the only possibilities in the cells they belong.
class ConstraintIII : public IConstraint
{
public:
	virtual bool Run(Board::TCellBasket& basket);
private:
	static Enroller<ConstraintIII> m_enroller;
};

class CSPSolver : public SolverBase
{
public:
	CSPSolver(const Board& board) : SolverBase(board) 
		{ LOG("Created solver: " + Name()); }
	//CSPSolver(const CSPSolver& solver) {}
	~CSPSolver() {}

	virtual Board& Solve();

private:
	void SolveRec(Board& board) const;
	void RunConstraints(Board& board) const;
};

#endif //_CSPSolver_H_INCLUDED_