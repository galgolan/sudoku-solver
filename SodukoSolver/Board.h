#ifndef _BOARD_H_INCLUDED_
#define _BOARD_H_INCLUDED_

#include <vector>
using namespace std;

#include <math.h>

#include "Cell.h"

class Board
{
public:
	typedef vector<Cell>		TCellRow;
	typedef vector<TCellRow>	TCellTable;
	typedef set<Cell*>			TCellBasket;
	typedef struct { int x,y; } TCellPos;

	enum EBoardStatus
	{
		BOARD_STATUS_SOLVED,
		BOARD_STATUS_UNSOLVED,
		BOARD_STATUS_STUCK
	};


	Board(int size);
	Board(const int* vals, unsigned int size);
	Board(const Board& board) : m_board(board.m_board), m_boardStatus(board.m_boardStatus) {}
	Board(const Board& board, int x, int y);

	void Fill(const int* vals);

	bool GetCellPos(const Cell* cell, TCellPos& pos) const;

	void UpdateStatus();
	EBoardStatus GetStatus() /*const*/ { UpdateStatus(); return m_boardStatus; }

	const TCellBasket& GetRegion(unsigned int x, unsigned int y);
	const TCellBasket& GetRow(unsigned int row);
	const TCellBasket& GetColumn(unsigned int col);

	Cell* GetCell(int x, int y) { return &(m_board[x])[y]; }
	size_t GetSize() const { return m_board.size(); }
	unsigned int GetRegionSize() const { return (unsigned int)sqrt((float)GetSize()); }

	//Returns:	-1		when the board is stuck.
	//			0 - 99	when the board is on its way to be solved.
	//					the progress will be returned.
	//			100		when the board is solved all the way.
	bool Verify(bool considerPossibilities = false);

	TCellRow& operator [] (unsigned int index);

private:
	TCellTable m_board;
	EBoardStatus m_boardStatus;

	bool VerifyBasket(const TCellBasket& basket) const;
};

ostream& operator << (ostream& os, const Board::TCellBasket& basket);
ostream& operator << (ostream& os, const Board& board);

#endif //_BOARD_H_INCLUDED_