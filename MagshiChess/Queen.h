#pragma once
#include "Bishop.h"
#include "Rook.h"

class Queen : public Piece {
public:
	Queen(Color color);
	~Queen();
	bool validMove(Piece*** board, std::string move);
private:
	Rook* _rook;
	Bishop* _bishop;
};