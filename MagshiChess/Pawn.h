#pragma once
#include "Piece.h"

#define MOVED_PAWN_MOVING_RANGE 1
#define UNMOVED_PAWN_MOVING_RANGE 2
#define PAWN_EATING_MOVE_RANGE 1

class Pawn : public Piece {
public:
	Pawn(Color c); 
	~Pawn();
	bool validMove(Piece*** board, std::string move);

private:
	bool _moved;
};

