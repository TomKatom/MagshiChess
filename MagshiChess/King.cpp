#include "King.h"
#include <cmath>        // std::abs
#include "PipeInputOperations.h"

King::King(Color color) : Piece(color)
{
	/*
Setting the pieceChar according to the color of the piece
*/
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
}

bool King::validMove(Piece*** board, std::tuple<int, int, int, int> positions)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;
	if (std::abs(srcCol - dstCol) <= MOVING_RANGE && std::abs(srcRow - dstRow) <= MOVING_RANGE)
		return true;
	else return false;
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