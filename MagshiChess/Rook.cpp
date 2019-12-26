#pragma once
#include "Rook.h"
#include "PipeInputOperations.h"
#include <tuple>

Rook::Rook(Color color) : Piece(color) {
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (color) {
		case Color::white:
			this->_pieceChar = 'R';
			break;
		case Color::black:
			this->_pieceChar = 'r';
			break;
	}
}
Rook::~Rook() {

}
bool Rook::validMove(Piece*** board, std::string move) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = PipeInputOperations::moveToPos(move);
	if (srcCol == dstCol) {
		if (srcRow < dstRow) {
			for (int i = srcRow + 1; i < dstRow; i++) {
				if (board[i][srcCol] != nullptr) {
					return false;
				}
			}
			return true;
		}
		else if (srcRow > dstRow) {
			for (int i = srcRow - 1; i > dstRow; i--) {
				if (board[i][srcCol] != nullptr) {
					return false;
				}
			}
			return true;
		}
	}
	else if (srcRow == dstRow) {
		if (srcCol < dstCol) {
			for (int i = srcCol + 1; i < dstCol; i++) {
				if (board[srcRow][i] != nullptr) {
					return false;
				}
			}
			return true;
		}
		else if (srcCol > dstCol) {
			for (int i = srcCol - 1; i > dstCol; i--) {
				if (board[srcRow][i] != nullptr) {
					return false;
				}
			}
			return true;
		}
	}
	else {
		return false;
	}
	return false;
}