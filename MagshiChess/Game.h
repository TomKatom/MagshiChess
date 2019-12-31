#pragma once
#include "Player.h"
#include <SFML/Network.hpp>
#include <mutex>

class Game
{
public:
	Game(std::string& startingBoard, Pipe &p, Pipe& change, std::mutex* mu, sf::TcpSocket* sock);
	~Game();
	void setCurrTurn(bool turn);
	Player* getPlayer();

	void playTurn(std::string& getMessageFromGraphics);

private:
	bool isValidCode(error_level_code e) const;
	Pipe _p;
	Pipe _change;
	bool _thisTurn;
	Player*  _whitePlayer;
	Player *_blackPlayer;
	std::mutex* _mu;
	sf::TcpSocket* _sock;
};

