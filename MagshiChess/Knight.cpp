#include "Knight.h"
#include "PipeInputOperations.h"

Knight::Knight(Color color) : Piece(color) {
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (color) {
		case Color::white:
			this->_pieceChar = 'N';
			break;
		case Color::black:
			this->_pieceChar = 'n';
			break;
	}
}
Knight::~Knight() {

}
bool Knight::validMove(Piece*** board,std::tuple<int, int, int, int> positions) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;
	if (dstCol - 2 == srcCol and dstRow - 1 == srcRow) { // Right Top Middle
		return true;
	}
	else if (dstCol - 2 == srcCol and srcRow - 1 == dstRow) { // Right Bottom Middle
		return true;
	}
	else if (dstCol - 1 == srcCol and dstRow - 2 == srcRow) { // Top Right
		return true;
	}
	else if (dstCol + 1 == srcCol and dstRow - 2 == srcRow) { // Top Left
		return true;
	}
	else if (dstCol + 2 == srcCol and dstRow - 1 == srcRow) { // Left Top Middle
		return true;
	}
	else if (dstCol + 2 == srcCol and dstRow + 1 == srcRow) { // Left Bottom Middle
		return true;
	}
	else if (dstCol + 1 == srcCol and dstRow + 2 == srcRow) { // Bottom Left
		return true;
	}
	else if (dstCol - 1 == srcCol and dstRow + 2 == srcRow) { // Bottom Right
		return true;
	}
	else {
		return false;
	}

}