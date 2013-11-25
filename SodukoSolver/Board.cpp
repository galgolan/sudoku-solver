#include <sstream>
using namespace std;

#include "Board.h"

#define LOG_SUBJECT	"Board"
#include "SudokuException.h"

Board::Board(const int* vals, unsigned int size) : m_board(size)
{
	for(unsigned int x = 0; x < size; ++x)
		for(unsigned int y = 0; y < size; ++y)
			m_board[x].push_back(Cell(vals[x*size + y], size));

	//UpdateStatus();

	stringstream ss;
	ss << "Board created, size: " << size;
	LOG(ss.str());
}

Board::Board(int size) : m_board(size)
{
	for(unsigned int x = 0; x < m_board.size(); ++x)
		for(unsigned int y = 0; y < m_board.size(); ++y)
			m_board[x].push_back(Cell(size));

	//UpdateStatus();

	stringstream ss;
	ss << "Board created, size: " << size;
	LOG(ss.str());
}

bool Board::GetCellPos(const Cell* cell, TCellPos& pos) const
{
	for(unsigned int x = 0; x < m_board.size(); ++x)
		for(unsigned y = 0; y < m_board[x].size(); ++y)
			if(&m_board[x][y] == cell)	// compare addresses to find the same cell
			{
				pos.y = y;
				pos.x = x;
				return true;
			}
	return false;
}

bool Board::Verify(bool considerPossibilities)
{
	for(unsigned int n = 0; n < m_board.size(); ++n)
	{
		if(!VerifyBasket(GetRow(n)))
			return false;
		else if(!VerifyBasket(GetColumn(n)))
			return false;
		else if(!VerifyBasket(GetRegion(n % GetRegionSize(), n / GetRegionSize())))
			return false;
	}
	return true;
}

bool Board::VerifyBasket(const TCellBasket& basket) const
{
	set<int> valSet;
	for(TCellBasket::const_iterator it = basket.begin(); it != basket.end(); ++it)
		if(((*it)->IsEmpty()) && ((*it)->GetPossibilities().size() == 0))
			return false;
		else
			valSet.insert((*it)->GetValue());
	return (valSet.size() == m_board.size());
}

Board::TCellRow& Board::operator [] (unsigned int index)
{
	if((m_board.size() <= index) || (index < 0))
		THROW("tried to access a row with an out-of-range index.");
	return m_board[index];
}

const Board::TCellBasket& Board::GetRegion(unsigned int x, unsigned int y)
{
	if((x < 0) || (x >= GetRegionSize()) || (y < 0) || (y >= GetRegionSize()))
		THROW("tried to access a region with an out-of range index.");

	static TCellBasket basket;
	basket.clear();
	
	for(unsigned int ix = 0; ix < GetRegionSize(); ++ix)
		for(unsigned int iy = 0; iy < GetRegionSize(); ++iy)
			basket.insert(&m_board[x*GetRegionSize() + ix][y*GetRegionSize() + iy]);
			
	return basket;
}

const Board::TCellBasket& Board::GetRow(unsigned int row)
{
	if((row < 0) || (row >= m_board.size()))
		THROW("tried to access a row with an out-of range index.");

	static TCellBasket basket;
	basket.clear();
	for(unsigned int i = 0; i < m_board.size(); ++i)
		basket.insert(&m_board[row][i]);
	return basket;
}

const Board::TCellBasket& Board::GetColumn(unsigned int col)
{
	if((col < 0) || (col >= m_board.size()))
		THROW("tried to access a column with an out-of range index.");

	static TCellBasket basket;
	basket.clear();
	for(unsigned int i = 0; i < m_board.size(); ++i)
		basket.insert(&m_board[i][col]);
	return basket;
}

ostream& operator << (ostream& os, const Board::TCellBasket& basket)
{
	for(Board::TCellBasket::const_iterator it = basket.begin(); it != basket.end(); ++it)
		os << **it << ',';
	return os;
}

ostream& operator << (ostream& os, const Board& board)
{
	for(unsigned int x = 0; x < board.GetSize(); ++x)
	{
		//separator
		for(unsigned int y = 0; y < board.GetSize() * 4 + 1; ++y)
				os << '-';
		os << '\n';
		for(unsigned int y = 0; y < board.GetSize(); ++y)
		{
			os << "| ";
			if(const_cast<Board&>(board)[x][y].GetValue() != 0)
				os << const_cast<Board&>(board)[x][y].GetValue();
			else
				os << ' ';
			os << ' ';
		}
		os << "|\n";
	}
	//separator
	for(unsigned int y = 0; y < board.GetSize() * 4 + 1; ++y)
		os << '-';	// separator

	return os;
}

void Board::Fill(const int* vals)
{
	if(m_board.size() <= 0)
		THROW("Board filled with an invalid size.");

	for(unsigned int i = 0; i < m_board.size() * m_board.size(); ++i)
		m_board[i / m_board.size()][i % m_board.size()] = vals[i];

	stringstream ss;
	ss << "Board filled, size: " << m_board.size();
	LOG(ss.str());
}

void Board::UpdateStatus()
{
	m_boardStatus = BOARD_STATUS_SOLVED;	//assume the board is solved
	for(unsigned int x = 0; x < m_board.size(); ++x)
		for(unsigned int y = 0; y < m_board.size(); ++y)
		{
			if(m_board[x][y].GetStatus() == Cell::CELL_STATUS_STUCK)
			{
				m_boardStatus = BOARD_STATUS_STUCK;
				return;
			}
			else if(m_board[x][y].GetStatus() == Cell::CELL_STATUS_UNSOLVED)
				m_boardStatus = BOARD_STATUS_UNSOLVED;
		}
}