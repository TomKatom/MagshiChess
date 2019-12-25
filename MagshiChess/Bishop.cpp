#include "Bishop.h"
#include "PipeInputOperations.h"
#include <tuple>

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
	if (dstRow - srcRow == dstCol - srcCol) {
		for (int i = srcRow + 1, j = srcCol + 1; i < dstCol; i++, j++) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	return false;
}