#include "King.h"
#include <cmath>        // std::abs
#include "PipeInputOperations.h"
#include <locale>

King::King(Color color, std::string& startingBoard, Color onlinePlayerColor) : Piece(color)
{
	/*
Setting the pieceChar according to the color of the piece
*/
	this->_moved = false;
	this->_pos = new int[2];

	if (onlinePlayerColor == Color::white)
	{
		if (color == Color::black)
		{
			this->_pos[0] = 7;
			this->_pos[1] = 4;
		}
		else
		{
			this->_pos[0] = 0;
			this->_pos[1] = 4;
		}
	}
	else
	{
		if (color == Color::black)
		{
			this->_pos[0] = 0;
			this->_pos[1] = 4;
		}
		else
		{
			this->_pos[0] = 7;
			this->_pos[1] = 4;
		}
	}

	switch (color) {
	case Color::white:
		this->_pieceChar = 'K';
		break;
	case Color::black:
		this->_pieceChar = 'k';
		break;
	}
}

King::~King()
{
	delete[] this->_pos;
}

int* King::getPos() 
{
	return this->_pos;
}

void King::setPos(int* new_pos)
{
	this->_pos[0] = new_pos[0];
	this->_pos[1] = new_pos[1];
}

bool King::validMove(Piece*** board, std::tuple<int, int, int, int> positions)
{
	auto [srcRow, srcCol, dstRow, dstCol] = positions;
	if (!(std::abs(srcCol - dstCol) <= MOVING_RANGE && std::abs(srcRow - dstRow) <= MOVING_RANGE))
		return false;
	
	this->_moved = true;
	return true;
}

bool King::isChecked(Piece*** board)
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (board[row][col] != nullptr && board[row][col]->getColor() != this->_color)
			{
				if (board[row][col]->validMove(board, std::make_tuple(row, col, this->_pos[0], this->_pos[1])))  //some piece can eat the king
					return true;
			}
		}
	}
	return false;
}

