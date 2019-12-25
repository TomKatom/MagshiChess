#include "Game.h"


//c'tor
Game::Game(std::string& startingBoard, Pipe& p)
{
	std::string revreseBoard = startingBoard;
	reverse(revreseBoard.begin(), revreseBoard.end() );

	this->_whitePlayer = new Player(white, revreseBoard);
	this->_blackPlayer = new Player(black, revreseBoard);
	this->_currentPlayerTurn = black;
	this->_p = p;
}

//d'tor
Game::~Game()
{
	delete this->_whitePlayer;
	delete this->_blackPlayer;
}

void Game::playTurn(std::string& messageFromGraphics)
{
	error_level_code move_code;
	
	if (this->_currentPlayerTurn == black)
	{
		move_code = this->_blackPlayer->isValidCMD(messageFromGraphics);
	}
	else
	{
		move_code = this->_whitePlayer->isValidCMD(messageFromGraphics);
	}
		char msgToGraphics[1024];
		msgToGraphics[0] = (char)(move_code + '0');
		msgToGraphics[1] = 0;

		this->_p.sendMessageToGraphics(msgToGraphics);

		if (move_code == valid || move_code == valid_check || move_code == check_mate)
		{
			//TODO: change it when converting to multyplayer
			this->_whitePlayer->makeMove(messageFromGraphics);
			this->_blackPlayer->makeMove(messageFromGraphics);


			if (this->_currentPlayerTurn == white)
				this->_currentPlayerTurn = black;
			else
				this->_currentPlayerTurn = white;
		}
	
}
