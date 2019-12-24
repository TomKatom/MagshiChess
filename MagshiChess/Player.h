#pragma once

#include <string>
#include "Piece.h"
#include "Pipe.h"
#include "PipeInputOperations.h"
#include <tuple>

class Player
{
public: 
	Player();    //c'tor
	~Player();   //d'tor

	void makeMove(std::string& msgFromGraphics);
	void setColor(Color& c);
	void setPipe(Pipe p); 

private:
	Piece*** _board;
	Color _color;
	Pipe _p;
	//King* _king;
	//SOCKET _server;  //TODO: Add class that communicate with the server
	//void connectToAServer(); //This function connect to server and get the color of the player (white/black)

	error_level_code isValidCMD(std::string& msgFromGraphics) const; //
	void createBoard();

};

