#define LOG_SUBJECT "BoardsBank"
#include "SudokuException.h"
#include "BoardsBank.h"
#include <sstream>
using namespace std;

BoardsBank::BoardsBank()
{
	int vals1[] = {
		0,4,0,0,2,0,0,5,0,
		0,0,0,4,0,8,0,0,0,
		7,2,0,5,0,9,0,1,4,
		0,9,4,0,7,0,6,2,0,
		8,0,0,2,0,5,0,0,3,
		0,5,2,0,4,0,9,7,0,
		5,7,0,6,0,1,0,8,2,
		0,0,0,7,0,4,0,0,0,
		0,8,0,0,5,0,0,6,0
	};
	m_boardBank.push_back(BoardRecord(Board(vals1, 9), LEVEL_VERY_EASY));

	int vals2[] = {
		0,8,0,0,9,0,0,6,0,
		0,0,0,4,6,0,0,5,2,
		5,0,0,0,0,0,4,0,0,
		0,0,0,0,1,9,0,0,0,
		0,0,2,0,0,0,7,0,0,
		1,0,0,7,5,0,0,0,3,
		0,0,3,0,0,0,0,0,0,
		2,9,0,0,8,7,0,0,4,
		0,7,0,0,4,0,6,0,0
	};
	m_boardBank.push_back(BoardRecord(Board(vals2, 9), LEVEL_MEDIUM));

	int vals3[] = {
		0,0,7,8,0,5,2,0,0,
		8,0,0,6,0,4,0,0,5,
		0,1,0,0,9,0,0,8,0,
		4,0,0,2,8,9,0,0,7,
		0,0,0,0,0,0,0,0,0,
		5,0,0,7,6,1,0,0,2,
		0,7,0,0,3,0,0,6,0,
		3,0,0,1,0,6,0,0,4,
		0,0,2,5,0,8,1,0,0
	};
	m_boardBank.push_back(BoardRecord(Board(vals3, 9), LEVEL_UNKNOWN));
	
	int vals4[] = {
		0,0,0,0,7,0,5,0,1,
		1,0,5,0,0,0,0,2,0,
		0,9,0,0,0,0,8,7,0,
		0,0,0,0,0,8,0,1,4,
		7,0,6,0,1,0,9,0,3,
		8,3,0,6,0,0,0,0,0,
		0,1,7,0,0,0,0,9,0,
		0,6,0,0,0,0,2,0,5,
		3,0,8,0,2,0,0,0,0
	};

	int vals5[] = {
		6,5,3,0,0,0,7,0,0,
		0,0,0,0,0,9,0,0,0,
		8,0,4,0,5,0,0,0,3,
		9,0,0,0,1,7,3,0,0,
		0,0,5,0,3,0,8,0,0,
		0,0,7,2,9,0,0,0,5,
		4,0,0,0,7,0,2,0,6,
		0,0,0,8,0,0,0,0,0,
		0,0,6,0,0,0,1,5,7
	};
	m_boardBank.push_back(BoardRecord(Board(vals5, 9), LEVEL_UNKNOWN));

	m_boardBank.push_back(BoardRecord(Board(vals4, 9), LEVEL_HARD));
	stringstream ss;
	ss << "Created Bank with: " << m_boardBank.size() << " boards.";
	LOG(ss.str());
}

const Board* BoardsBank::GetBoard(EBoardDifficultyLevel level, unsigned int index, int size) const
{
	int levelMatches = -1;
	for(unsigned int i = 0; i < m_boardBank.size(); ++i)
	{
		if((m_boardBank[i].m_level == level) && (++levelMatches == index))
			if((size != -1) && (m_boardBank[i].m_board.GetSize() == size) || (size == -1))
				return &(m_boardBank[i].m_board);
	}
	// no board was found
	return NULL;
}