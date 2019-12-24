#pragma once
#include "Piece.h"

Piece::Piece(Color color) {
	this->_color = color;
}
Color Piece::getColor() const{
	return this->_color;
}
Piece::~Piece() {

}
char Piece::getPieceChar() const {
	return this->_pieceChar;
}