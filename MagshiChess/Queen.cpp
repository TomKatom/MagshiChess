#include "Queen.h"
Queen::Queen(Color color) : Piece(color)
{
	this->_rook = new Rook(color);
	this->_bishop = new Bishop(color);
	switch (color) {
	case Color::white:
		this->_pieceChar = 'Q';
		break;
	case Color::black:
		this->_pieceChar = 'q';
		break;
	}
}
Queen::~Queen() {
	delete this->_rook;
	delete this->_bishop;
}
bool Queen::validMove(Piece*** board, std::string move) {
	return (this->_bishop->validMove(board, move) or this->_rook->validMove(board, move));
}