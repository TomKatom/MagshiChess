#include "Bishop.h"
#include "PipeInputOperations.h"
#include <tuple>
#include <cmath>

Bishop::Bishop(Color color) : Piece(color){
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (color) {
		case white:
			this->_pieceChar = 'B';
			break;
		case black:
			this->_pieceChar = 'b';
			break;
	}
}
Bishop::~Bishop() {

}
bool Bishop::validMove(Piece*** board, std::string move) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = PipeInputOperations::moveToPos(move);
	if (dstRow - srcRow == dstCol - srcCol and dstCol > srcCol and dstRow > srcRow ) { // Top Right Diagnoal
		for (int i = srcRow + 1, j = srcCol + 1; i < dstRow; i++, j++) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol < srcCol and srcRow < dstRow) { // Top Left Diagnoal
		for (int i = srcRow + 1, j = srcCol - 1; i < dstRow; i++, j--) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol < srcCol and srcRow > dstRow) { // Bottom Left Diagnoal
		for (int i = srcRow - 1, j = srcCol - 1; i > dstRow; i--, j--) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol > srcCol and srcRow > dstRow) { // Bottom Right Diagnoal
		for (int i = srcRow - 1, j = srcCol + 1; i > dstRow; i--, j++) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	return false;
}