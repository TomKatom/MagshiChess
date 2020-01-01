/*
This file servers as an example of how to use Pipe.h file.
It is recommended to use the following code in your project,
in order to read and write information from and to the Backend
*/

#include "Pipe.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>
#include <mutex>
#pragma warning(disable:4996)

std::mutex* mu;

using std::cout;
using std::endl;
using std::string;

void serverListener(sf::TcpSocket* sock, Pipe chatPipe, Pipe changePipe, Game* g)
{
	char data[10240] = { 0 };
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0, guiDstRow = 0, guiSrcRow = 0;
	char crown = 0;
	string msg = "";
	string value = "";
	std::size_t received;
	while (true) {
		sock->receive(data, 10240, received);
		msg = string(data);
		if (msg.find("chat") != string::npos) {
			std::cout << "Got msg cmd";
			msg = "chat " + string(data);
			strcpy(data, msg.c_str());
			changePipe.sendMessageToGraphics(data);
			cout << "Received data " << endl;
			std::cout << "Sent to pipe: " << msg << endl;
		} // move 1,1 3,3
		else if (msg.find("move") != string::npos) {
			std::unique_lock<std::mutex> lock(*mu);
			if (msg.length() > 14) {
				strcpy(data, "msg 1");
				changePipe.sendMessageToGraphics(data);
			}
			srcRow = (msg[5] - '0');
			guiSrcRow = 7 - srcRow;
			srcCol = (msg[7] - '0');
			guiDstRow = 7 - (msg[9] - '0');
			dstRow = (msg[9] - '0');
			dstCol = (msg[11] - '0');
			g->getOtherPlayer()->makeMove(std::make_tuple(srcRow, srcCol, dstRow, dstCol));
			g->getOnlinePlayer()->makeMove(std::make_tuple(srcRow, srcCol, dstRow, dstCol));
			msg = "change ";
			msg += (char)(guiSrcRow + '0'); 
			msg += ','; 
			msg += (char)(srcCol + '0'); 
			msg += ' ';
			msg += '#';
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			changePipe.sendMessageToGraphics(data);
			msg = "change "; 
			msg += (char)(guiDstRow + '0');
			msg += ',';
			msg += (char)(dstCol + '0');
			msg += ' ';
			msg += g->getOtherPlayer()->getBoard()[dstRow][dstCol]->getPieceChar();
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			changePipe.sendMessageToGraphics(data);
			strcpy(data, "turn");
			changePipe.sendMessageToGraphics(data);
			g->setCurrTurn(true);
		}
		else if (msg.find("mate") != string::npos) {
			if (g->getOnlinePlayer()->getColor() == Color::black) {
				strcpy(data, "mate black");
				changePipe.sendMessageToGraphics(data);
			}
			else if (g->getOnlinePlayer()->getColor() == Color::white) {
				strcpy(data, "mate white");
				changePipe.sendMessageToGraphics(data);
			}
		}
		else if (msg.find("crown") != string::npos) {
			std::unique_lock<std::mutex> lock(*mu);
			crown = msg[10];
			srcRow = msg[6] - '0';
			srcCol = msg[8] - '0';
			guiSrcRow = 7 - srcRow;
			delete g->getOnlinePlayer()->getBoard()[srcRow][srcCol];
			g->getOnlinePlayer()->getBoard()[srcRow][srcCol] = PipeInputOperations::getPieceFromChar(crown);
			delete g->getOtherPlayer()->getBoard()[srcRow][srcCol];
			g->getOtherPlayer()->getBoard()[srcRow][srcCol] = PipeInputOperations::getPieceFromChar(crown);
			msg = "change ";
			msg += (char)guiSrcRow + '0';
			msg += ',';
			msg += (char)srcCol + '0';
			msg += ' ';
			msg += crown;
			strcpy(data, msg.c_str());
			changePipe.sendMessageToGraphics(data);
		}
	}
}
void chatPipeListener(sf::TcpSocket* sock, Pipe chatPipe) {
	string msg = "";
	while (true) {
		msg = chatPipe.getMessageFromGraphics();
		cout << "Received From Pipe: " << msg << endl;
		sock->send(msg.c_str(), msg.length() + 1);
		cout << "Sent to server: " << msg << endl;
	}
}

int main()
{
	mu = new std::mutex;
	srand(time_t(NULL));
	std::string str4gui = "";  //white - check
	std::string str4game ="";

	//std::string str4gui = "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr0";  //black - check
	//std::string str4game = "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr";

	//ayer playerPl(white);
	char data[10240] = { 0 };
	std::size_t receieved;
	Pipe p(0);
	Pipe change(1);
	Pipe chat(2);
	bool isConnectP = p.connect();
	bool isConnectChange = change.connect();
	sf::TcpSocket* sock = new sf::TcpSocket();
	Game* g = nullptr;
	string ans;

	while (!isConnectP && isConnectChange)
	{
		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			Sleep(5000);
			isConnectP = p.connect();
			isConnectChange = change.connect();
		}
		else
		{
			p.close();
			return 0;
		}
		return 0;
	}
	char msgToGraphics[10240];
	sock->connect("192.168.1.32", 5000);
	chat.connect();
	sock->receive(data, 10240, receieved);
	if (std::string(data).find("wait") != std::string::npos) 
	{
		strcpy(msgToGraphics, "wait");
		sock->receive(data, 10240, receieved);
		if (std::string(data).find("connect") != std::string::npos)
		{	//white
			strcpy(msgToGraphics, "connect");
			p.sendMessageToGraphics(msgToGraphics);
			str4gui = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
			str4game ="rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";

			g = new Game(str4game, p, change, mu, sock, Color::white);
			g->setCurrTurn(true);
		}
	}
	else {  //black
		strcpy(msgToGraphics, "connect");
		p.sendMessageToGraphics(msgToGraphics);
		str4gui =  "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr0";
		str4game = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";

		g = new Game(str4game, p, change, mu, sock, Color::black);
		g->setCurrTurn(false);
	}
	std::thread(serverListener, sock, chat, change, g).detach();
	std::thread(chatPipeListener, sock, chat).detach();

	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, str4gui.c_str()); // just example...

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();

	while (msgFromGraphics != "quit")
	{
		g->playTurn(msgFromGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
	return 0;
}