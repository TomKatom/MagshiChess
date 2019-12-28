#include "Game.h"
#include "PipeInputOperations.h"
#pragma warning(disable:4996)

//c'tor
Game::Game(std::string& startingBoard, Pipe& p, Pipe& change) : _p(p), _change(change)
{
	std::string revreseBoard = startingBoard;
	reverse(revreseBoard.begin(), revreseBoard.end() );

	this->_whitePlayer = new Player(Color::white, revreseBoard);
	this->_blackPlayer = new Player(Color::black, revreseBoard);
	this->_currentPlayerTurn = Color::white;
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
	int srcRow, srcCol, dstRow, dstCol;
	auto positions = PipeInputOperations::moveToPos(messageFromGraphics);
	std::tie(srcRow, srcCol, dstRow, dstCol) = positions;
	bool choose_white = false;
	bool choose_black = false;
	string result = "";
	string msg;
	Piece* curr;
	bool notMate = false;
	bool mate = false;
	std::tuple<bool, Piece*> eaten;
	if (this->_currentPlayerTurn == Color::black)
	{
		move_code = this->_blackPlayer->isValidCMD(positions);
	}
	else
	{
		move_code = this->_whitePlayer->isValidCMD(positions);
	}

		char msgToGraphics[1024];

		
		if (move_code == valid || move_code == valid_check || move_code == check_mate)
		{
			//TODO: change it when converting to multyplayer
			this->_whitePlayer->makeMove(positions);
			this->_blackPlayer->makeMove(positions);

			if (this->_currentPlayerTurn == Color::white)
			{
				this->_currentPlayerTurn = Color::black;
				if (this->_blackPlayer->getKing()->isChecked(this->_blackPlayer->getBoard())) {
					move_code = valid_check;
					for (int i = 0; i < 8 and !notMate; i++) {
						for (int j = 0; j < 8 and !notMate; j++) {
							curr = this->_whitePlayer->getBoard()[i][j];
							for (int k = 0; k < 8 and !notMate; k++) {
								for (int l = 0; l < 8 and !notMate; l++) {
									if (this->_whitePlayer->getBoard()[k][l] != curr and curr != nullptr) {
										if (this->_whitePlayer->isValidCMD(std::tuple<int, int, int, int>(i, j, k, l))) {
											eaten = this->_whitePlayer->makeMove(std::tuple<int, int, int, int>(i, j, k, l));
											if (!this->_blackPlayer->getKing()->isChecked(this->_whitePlayer->getBoard())) {
												notMate = true;
												mate = false;
											}
											else {
												mate = true;
											}
											this->_whitePlayer->undoMove(std::tuple<int, int, int, int>(i, j, k, l), eaten);
										}
									}
								}
							}
						}
					}
				}
				if (mate) {
					strcpy(msgToGraphics, "mate black");
					this->_change.sendMessageToGraphics(msgToGraphics);
					_exit(0);
				}
			}
			else
			{
				this->_currentPlayerTurn = Color::white;
				if (this->_whitePlayer->getKing()->isChecked(this->_whitePlayer->getBoard())) {

					move_code = valid_check;
					for (int i = 0; i < 8 and !notMate; i++) {
						for (int j = 0; j < 8 and !notMate; j++) {
							curr = this->_blackPlayer->getBoard()[i][j];
							for (int k = 0; k < 8 and !notMate; k++) {
								for (int l = 0; l < 8 and !notMate; l++) {
									if (this->_blackPlayer->getBoard()[k][l] != curr and this->_blackPlayer->getBoard() != nullptr and curr != nullptr) {
										if (curr->validMove(this->_whitePlayer->getBoard(), std::tuple<int, int, int, int>(i, j, k, l))) {
											eaten = this->_blackPlayer->makeMove(std::tuple<int, int, int, int>(i, j, k, l));
											if (!this->_whitePlayer->getKing()->isChecked(this->_blackPlayer->getBoard())) {
												notMate = true;
												mate = false;
											}
											else {
												mate = true;
											}
											this->_blackPlayer->undoMove(std::tuple<int, int, int, int>(i, j, k, l), eaten);
										}
									}
								}
							}
						}
					}
				}
				if (mate) {
					strcpy(msgToGraphics, "mate white");
					this->_change.sendMessageToGraphics(msgToGraphics);
					_exit(0);
				}
			}
			if (this->_whitePlayer->getBoard()[dstRow][dstCol]->getPieceChar() == 'P' and dstRow == 7) {
				choose_white = true;
			}
			else if (this->_whitePlayer->getBoard()[dstRow][dstCol]->getPieceChar() == 'p' and dstRow == 0) {
				choose_black = true;
			}
		}

		msgToGraphics[0] = (char)(move_code + '0');
		msgToGraphics[1] = 0;
		this->_p.sendMessageToGraphics(msgToGraphics);
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
}

