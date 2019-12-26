#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
	Knight(Color color);
	~Knight();
	bool validMove(Piece*** board, std::string move);
};
