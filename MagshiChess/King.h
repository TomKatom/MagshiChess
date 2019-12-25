#pragma once
#include "Piece.h"
#define MOVING_RANGE 1
class King : public Piece
{
public:
	King(Color color);
	~King();

	virtual bool validMove(Piece*** board, std::string move);
	//virtual isChecked();

private:
	bool _moved;
	bool _checked;
};

