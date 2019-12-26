#include "King.h"
#include <cmath>        // std::abs
#include <tuple>
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

bool King::validMove(Piece*** board, std::string move)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = PipeInputOperations::moveToPos(move);
	if (std::abs(srcCol - dstCol) <= MOVING_RANGE && std::abs(srcRow - dstRow) <= MOVING_RANGE)
		return true;
	else return false;
}