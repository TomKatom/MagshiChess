#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
	Bishop(Color color);
	~Bishop();
	bool validMove(Piece*** board, std::string move);
};
