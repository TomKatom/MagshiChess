#pragma once

#include <string>
#include "Piece.h"
#include "Pipe.h"
#include "PipeInputOperations.h"
#include "King.h"

class Player
{
public: 
	Player(Color c, std::string startingBoard);    //c'tor
	~Player();   //d'tor

	std::tuple<bool, Piece*> makeMove(std::tuple<int, int, int, int> positions);
	void undoMove(std::tuple<int, int, int, int>positions, std::tuple<bool, Piece*> eatenPiece);
	King* getKing();
	Color getColor() const;
	Piece*** getBoard();
	error_level_code isValidCMD(std::tuple<int, int, int, int> positions);
	bool isChecked(Piece*** board, std::tuple<int, int, int, int> positions);

private:
	Piece*** _board;
	Color _color;
	King* _king;
	//SOCKET _server;  //TODO: Add class that communicate with the server
	//void connectToAServer(); //This function connect to server and get the color of the player (white/black)

};

