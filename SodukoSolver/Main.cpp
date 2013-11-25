#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#include "SudokuException.h"
#include "CSPSolver.h"
#include "BruteForceSolver.h"
#include "BoardsBank.h"
#include "PerfCounter.h"

void main()
{
	try
	{
		//PerfCounter::Init();

		BoardsBank boardsBank;
		const Board* origBoard = boardsBank.GetBoard(BoardsBank::LEVEL_UNKNOWN, 1);
		//const Board* origBoard = boardsBank.GetBoard(BoardsBank::LEVEL_VERY_HARD);
		SolverBase* solver = new CSPSolver(*origBoard);

		cout << "\n# Initial Board:\n" << *origBoard << endl;

		PERF("Solving the board",
			Board& solvedBoard = solver->Solve());

		PERF("Verifying the board",
			bool boardSolved = solvedBoard.Verify());

		if(boardSolved)
			cout << "Solved!" << endl;
		else
			cout << "Not Solved!" << endl;
		cout << "\n# Solved Board:\n" << solvedBoard << endl;
 
		delete solver;

		char c;
		cout << "\n-> Would you like to see the log file? ";
		cin >> c;
		if((c == 'y') || (c == 'Y'))
		{
			SudokuLogger::CloseLog();
			system(SudokuLogger::GetFilename().c_str());
		}
	}
	catch(exception& ex)
	{
		cout << ex.what() << endl;
	}
	catch(...)
	{
		cout << "Caught an unknown exception!" << endl;
		LOG("Caught an unknown exception!");
	}
}