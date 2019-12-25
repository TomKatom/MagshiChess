#include "Pawn.h"
#include "PipeInputOperations.h"
#include <tuple>
#include <cmath>        // std::abs

Pawn::Pawn(Color c) : Piece(c)
{
	this->_moved = false;
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (c) {
	case white:
		this->_pieceChar = 'R';
		break;
	case black:
		this->_pieceChar = 'r';
		break;
	}
}

Pawn::~Pawn()
{
}

bool Pawn::validMove(Piece*** board, std::string move)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = PipeInputOperations::moveToPos(move);

	if (board[dstRow][dstCol] != nullptr && board[dstRow][dstCol]->getColor() != this->_color)  //when eating enemy piece
	{			
		if ((this->_color == white && (dstRow - srcRow)== MOVED_PAWN_MOVING_RANGE) ||
			(this->_color == black && (srcRow - dstRow) == MOVED_PAWN_MOVING_RANGE))
		{
			if (std::abs(srcCol - dstCol) == PAWN_EATING_MOVE_RANGE) {
				if (!this->_moved) this->_moved = true;  //update bool var _moved
				return true;
			}
			else return false;

		}
		else return false;

	}
	else if(board[dstRow][dstCol] == nullptr)
	{
		if (srcCol == dstCol)
		{
			if (this->_moved) //piece moved already
			{
				if ((this->_color == white && (dstRow - srcRow) == MOVED_PAWN_MOVING_RANGE) ||
					(this->_color == black && (srcRow - dstRow) == MOVED_PAWN_MOVING_RANGE))
					return true;
				else return false;
			}
			else  //first step
			{
				if ((this->_color == white && ((dstRow - srcRow) == MOVED_PAWN_MOVING_RANGE || (dstRow - srcRow) == UNMOVED_PAWN_MOVING_RANGE)) ||
					(this->_color == black && ((srcRow - dstRow) == MOVED_PAWN_MOVING_RANGE || (srcRow - dstRow) == UNMOVED_PAWN_MOVING_RANGE)) )
				{
					this->_moved = true;
					return true;
				}
				else return false;
			}
		}
		else
			return false;
	}
	else
		return false;
}