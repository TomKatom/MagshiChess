#include "Game.h"
#include "PipeInputOperations.h"
#pragma warning(disable:4996)
#include <thread>
//c'tor
Game::Game(std::string& startingBoard, Pipe& p, Pipe& change, std::mutex* mu, sf::TcpSocket* sock) : _p(p), _change(change)
{
	std::string revreseBoard = startingBoard;
	reverse(revreseBoard.begin(), revreseBoard.end() );

	this->_whitePlayer = new Player(Color::white, revreseBoard);
	this->_blackPlayer = new Player(Color::black, revreseBoard);
	this->_thisTurn = false;
	this->_mu = mu;
	this->_sock = sock;
}

//d'tor
Game::~Game()
{
	delete this->_whitePlayer;
	delete this->_blackPlayer;
}

void Game::setCurrTurn(bool turn) {
	this->_thisTurn = turn;
}

bool Game::isValidCode(error_level_code e) const
{
	if (!(e == valid || e == valid_check ))
		return false;
	return true;
}

void Game::playTurn(std::string& messageFromGraphics)
{
	error_level_code move_code;
	auto positions = PipeInputOperations::moveToPos(messageFromGraphics);
	auto [srcRow, srcCol, dstRow, dstCol] = positions;
	bool choose_white = false;
	bool choose_black = false;
	string result = "";
	string msg;
	Piece* curr;
	bool notMate = false;
	Player* checkedPlayer = nullptr;
	bool mate = false;
	std::tuple<bool, Piece*> eaten;

	char msgToGraphics[1024];

	if (this->_thisTurn) {
		move_code = this->_whitePlayer->isValidCMD(positions);
		if (isValidCode(move_code))
		{
			std::unique_lock<std::mutex> lock(*_mu);
			//TODO: change it when converting to multyplayer
			this->_whitePlayer->makeMove(positions);
			this->_blackPlayer->makeMove(positions);

			this->_thisTurn = false;

			if (this->_blackPlayer->getKing()->isChecked(this->_blackPlayer->getBoard()))
				checkedPlayer = this->_blackPlayer;

			else if (this->_whitePlayer->getKing()->isChecked(this->_whitePlayer->getBoard()))
				checkedPlayer = this->_whitePlayer;

			if (checkedPlayer != nullptr)
			{

				move_code = valid_check;
				for (int i = 0; i < 8 and !notMate; i++) {
					for (int j = 0; j < 8 and !notMate; j++) {
						curr = checkedPlayer->getBoard()[i][j];
						for (int k = 0; k < 8 and !notMate; k++) {
							for (int l = 0; l < 8 and !notMate; l++) {
								if (checkedPlayer->getBoard()[k][l] != curr and curr != nullptr and curr->getColor() == checkedPlayer->getColor()) {
									if (isValidCode(checkedPlayer->isValidCMD(std::make_tuple(i, j, k, l)))) {
										eaten = checkedPlayer->makeMove(std::tuple<int, int, int, int>(i, j, k, l));
										if (!checkedPlayer->getKing()->isChecked(checkedPlayer->getBoard())) {
											notMate = true;
											mate = false;
										}
										else {
											mate = true;
										}
										checkedPlayer->undoMove(std::make_tuple(i, j, k, l), eaten);
									}
									else mate = false;
								}
							}
						}
					}
				}

				if (mate) {
					move_code = check_mate;
					if(checkedPlayer->getColor() == Color::black)
						strcpy(msgToGraphics, "mate black");
					else 
						strcpy(msgToGraphics, "mate white");
					this->_change.sendMessageToGraphics(msgToGraphics);
				//_exit(0);
				}
			}

		}

			if (this->_whitePlayer->getBoard()[dstRow][dstCol] != nullptr and this->_whitePlayer->getBoard()[dstRow][dstCol]->getPieceChar() == 'P' and dstRow == 7) {
				choose_white = true;
			}
			else if (this->_whitePlayer->getBoard()[dstRow][dstCol] != nullptr and this->_whitePlayer->getBoard()[dstRow][dstCol]->getPieceChar() == 'p' and dstRow == 0) {
				choose_black = true;
			}

			
			msgToGraphics[0] = (char)(move_code + '0');
			msgToGraphics[1] = 0;
			this->_p.sendMessageToGraphics(msgToGraphics);
			if (move_code == error_level_code::valid) {
				msg = "move " + (char)(srcRow + '0') + ',' + (char)(srcCol + '0') + ' ' + (char)(dstRow + '0') + ',' + (char)(dstCol + '0');
				this->_sock->send(msg.c_str(), msg.length() + 1);
			}
			if (choose_white) {
				result = "";
				strcpy(msgToGraphics, "choice white");
				this->_change.sendMessageToGraphics(msgToGraphics);
				while (result == "") {
					result = this->_p.getMessageFromGraphics();
				}
				switch (result[0]) {
				case 'Q':
					delete this->_blackPlayer->getBoard()[dstRow][dstCol];
					this->_blackPlayer->getBoard()[dstRow][dstCol] = new Queen(Color::white);
					delete this->_whitePlayer->getBoard()[dstRow][dstCol];
					this->_whitePlayer->getBoard()[dstRow][dstCol] = new Queen(Color::white);
					break;
				case 'R':
					delete this->_blackPlayer->getBoard()[dstRow][dstCol];
					this->_blackPlayer->getBoard()[dstRow][dstCol] = new Rook(Color::white);
					delete this->_whitePlayer->getBoard()[dstRow][dstCol];
					this->_whitePlayer->getBoard()[dstRow][dstCol] = new Rook(Color::white);
					break;
	
				case 'B':
					delete this->_blackPlayer->getBoard()[dstRow][dstCol];
					this->_blackPlayer->getBoard()[dstRow][dstCol] = new Bishop(Color::white);
					delete this->_whitePlayer->getBoard()[dstRow][dstCol];
					this->_whitePlayer->getBoard()[dstRow][dstCol] = new Bishop(Color::white);
					break;
				case 'N':
					delete this->_blackPlayer->getBoard()[dstRow][dstCol];
					this->_blackPlayer->getBoard()[dstRow][dstCol] = new Knight(Color::white);
					delete this->_whitePlayer->getBoard()[dstRow][dstCol];
					this->_whitePlayer->getBoard()[dstRow][dstCol] = new Knight(Color::white);
					break;
				}
			}
			else if (choose_black) {
				result = "";
				strcpy(msgToGraphics, "choice black");
				this->_change.sendMessageToGraphics(msgToGraphics);
				while (result == "") {
					result = this->_p.getMessageFromGraphics();
				}
				switch (result[0]) {
				case 'q':
					delete this->_blackPlayer->getBoard()[dstRow][dstCol];
					this->_blackPlayer->getBoard()[dstRow][dstCol] = new Queen(Color::black);
					delete this->_whitePlayer->getBoard()[dstRow][dstCol];
					this->_whitePlayer->getBoard()[dstRow][dstCol] = new Queen(Color::black);
					break;
				case 'r':
				delete this->_blackPlayer->getBoard()[dstRow][dstCol];
				this->_blackPlayer->getBoard()[dstRow][dstCol] = new Rook(Color::black);
				delete this->_whitePlayer->getBoard()[dstRow][dstCol];
				this->_whitePlayer->getBoard()[dstRow][dstCol] = new Rook(Color::black);
				break;

			case 'b':
				delete this->_blackPlayer->getBoard()[dstRow][dstCol];
				this->_blackPlayer->getBoard()[dstRow][dstCol] = new Bishop(Color::black);
				delete this->_whitePlayer->getBoard()[dstRow][dstCol];
				this->_whitePlayer->getBoard()[dstRow][dstCol] = new Bishop(Color::black);
				break;
			case 'n':
				delete this->_blackPlayer->getBoard()[dstRow][dstCol];
				this->_blackPlayer->getBoard()[dstRow][dstCol] = new Knight(Color::black);
				delete this->_whitePlayer->getBoard()[dstRow][dstCol];
				this->_whitePlayer->getBoard()[dstRow][dstCol] = new Knight(Color::black);
				break;
			}
		}
		if (choose_black or choose_white) {
			msg = "change ";
			msg += (char)(7 - dstRow + '0');
			msg += ',';
			msg += (char)(dstCol + '0');
			msg += ' ';
			msg += result[0];
			strcpy(msgToGraphics, msg.c_str());
			this->_change.sendMessageToGraphics(msgToGraphics);
		}
		_mu->unlock();
	}
	else {
		msgToGraphics[0] = (char)(2 + '0');
		msgToGraphics[1] = 0;
	}
	
}
Player* Game::getPlayer() {
	return this->_whitePlayer;
}
