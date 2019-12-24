#pragma once
#include <iostream>
enum Color {white, black};
class Piece {
public:
	Piece(Color color);
	~Piece();
	virtual bool validMove(Piece** board, std::string move) = 0;
	Color getColor() const;
	virtual char getPieceChar() const = 0;
private:
	Color _color;
	char _pieceChar;
};