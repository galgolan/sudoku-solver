#ifndef _BOARDSBANK_H_INCLUDED_
#define _BOARDSBANK_H_INCLUDED_
#include "Board.h"

#include "vector"
using namespace std;

class BoardsBank
{
public:
	enum EBoardDifficultyLevel
	{
		LEVEL_VERY_EASY,
		LEVEL_EASY,
		LEVEL_MEDIUM,
		LEVEL_HARD,
		LEVEL_VERY_HARD,
		LEVEL_UNKNOWN
	};

	BoardsBank();
	const Board* GetBoard(EBoardDifficultyLevel, unsigned int index = 0, int size = -1) const;

private:
	struct BoardRecord
	{
		BoardRecord(const Board& board, EBoardDifficultyLevel level)
			: m_board(board), m_level(level) {}

		Board m_board;
		EBoardDifficultyLevel m_level;
	};
	typedef vector<BoardRecord> TBoardBank;

	TBoardBank m_boardBank;
};

#endif //_BOARDSBANK_H_INCLUDED_