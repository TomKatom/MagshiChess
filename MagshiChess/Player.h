#pragma once

#include <string>
#include "Piece.h"
#include "Pipe.h"
#include "PipeInputOperations.h"
#include <tuple>

class Player
{
public: 
	Player(Color c, std::string startingBoard);    //c'tor
	~Player();   //d'tor

	void makeMove(std::string& msgFromGraphics);

	error_level_code isValidCMD(std::string& msgFromGraphics) const; //


private:
	Piece*** _board;
	Color _color;
	King* _king;
	//SOCKET _server;  //TODO: Add class that communicate with the server
	//void connectToAServer(); //This function connect to server and get the color of the player (white/black)

};

