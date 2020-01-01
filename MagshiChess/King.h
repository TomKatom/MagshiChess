#pragma once
#include "Piece.h"
#include <string>
#include <tuple>

#define MOVING_RANGE 1
class King : public Piece
{
public:
	King(Color color, std::string& startingBoard, Color onlinePlayerColor);
	~King();

	virtual bool validMove(Piece*** board, std::tuple<int, int, int, int> positions);
	virtual bool isChecked(Piece*** board);

	void setPos(int* new_pos);
	int* getPos() ;

private:
	bool _moved;
	//bool _checked;
	int* _pos; //pos[0] = row, pos[1] = col   
};