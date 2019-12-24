#pragma once
#include "Player.h"

class Game
{
public:
	Game(std::string& startingBoard, Pipe &p);
	~Game();

	void playTurn(std::string& getMessageFromGraphics);

private:
	Pipe _p;
	Color _currentPlayerTurn;
	Player*  _whitePlayer;
	Player *_blackPlayer;
};

