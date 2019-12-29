#pragma once
#include "Player.h"

class Game
{
public:
	Game(std::string& startingBoard, Pipe &p, Pipe& change);
	~Game();

	void playTurn(std::string& getMessageFromGraphics);

private:
	bool isValidCode(error_level_code e) const;
	Pipe _p;
	Pipe _change;
	Color _currentPlayerTurn;
	Player*  _whitePlayer;
	Player *_blackPlayer;
};

