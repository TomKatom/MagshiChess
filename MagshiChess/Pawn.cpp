#include "Pawn.h"
#include "PipeInputOperations.h"
#include <cmath>        // std::abs

Pawn::Pawn(Color c) : Piece(c)
{
	this->_moved = false;
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (c) {
	case Color::white:
		this->_pieceChar = 'P';
		break;
	case Color::black:
		this->_pieceChar = 'p';
		break;
	}
}

Pawn::~Pawn()
{
}

bool Pawn::validMove(Piece*** board,std::tuple<int,int,int,int> positions)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;

	if (board[dstRow][dstCol] != nullptr && board[dstRow][dstCol]->getColor() != this->_color)  //when eating enemy piece
	{			
		if ((dstRow - srcRow) != MOVED_PAWN_MOVING_RANGE)
		{
			return false;
		}
		else{
			if (std::abs(dstRow - srcRow) == PAWN_EATING_MOVE_RANGE) {
				if (!this->_moved) this->_moved = true;  //update bool var _moved
				return true;
			}
			else return false;

		}

	}
	if (board[dstRow][dstCol] != nullptr)
		return false;
	else
	{
		if (srcCol != dstCol)
			return false;
		else
		{
			if (this->_moved) //piece moved already
			{
				if ((dstRow - srcRow) != MOVED_PAWN_MOVING_RANGE)
					return false;
				else
					return false;
			}
			else  //first step
			{
				if ((dstRow - srcRow) < MOVED_PAWN_MOVING_RANGE || (dstRow - srcRow) > UNMOVED_PAWN_MOVING_RANGE)
					return false;
				else
				{
					this->_moved = true;
					return true;
				}
			}
		}
	}
}