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
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	string msg = "";
	string value = "";
	std::size_t received;
	while (true) {
		sock->receive(data, 10240, received);
		msg = string(data);
		if (msg.find("chat") != string::npos) {
			strcpy(data, msg.c_str());
			cout << "Received data " << endl;
			std::cout << "Sent to pipe: " << msg << endl;
		} // move 1,1 3,3
		if (msg.find("move") != string::npos) {
			std::unique_lock<std::mutex> lock(*mu);
			srcRow = msg[5] - '0';
			srcCol = msg[7] - '0';
			dstRow = msg[9] - '0';
			dstCol = msg[11] - '0';
			g->getPlayer()->makeMove(std::tuple<int, int, int, int>(srcRow, srcCol, dstRow, dstCol));
			msg = "change " + (char)(srcRow + '0') + ',' + (char)(srcCol + '0') + '#';
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			chatPipe.sendMessageToGraphics(data);
			msg = "change " + (char)(dstRow + '0') + ',' + (char)(dstCol + '0') + g->getPlayer()->getBoard()[dstRow][dstCol]->getPieceChar();
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			mu->unlock();
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
	//std::string str4game = "RNBKQBNRPPPPPPPP################################pppppppprnbKqbnr";

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
	char msgToGraphics[1024];
	sock->connect("127.0.0.1", 5000);
	chat.connect();
	sock->receive(data, 10240, receieved);
	if (std::string(data).find("wait") != string::npos) {
		strcpy(msgToGraphics, "wait");
		sock->receive(data, 10240, receieved);
		if (std::string(data).find("connect") != string::npos) {
		}
		sock->receive(data, 10240, receieved);
		str4gui = std::string(data);
		sock->receive(data, 10240, receieved);
		str4game = std::string(data);
		g = new Game(str4game, p, change, mu, sock);
		g->setCurrTurn(true);
	}
	else {
		str4gui = std::string(data);
		sock->receive(data, 10240, receieved);
		str4game = std::string(data);
		g = new Game(str4game, p, change, mu, sock);
		g->setCurrTurn(false);
	}



	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, str4gui.c_str()); // just example...

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();

	while (msgFromGraphics != "quit")
	{
		// should handle the string the sent from graphics
		// according the protocol. Ex: e2e4           (move e2 to e4)

		// YOUR CODE
		//strcpy_s(msgToGraphics, player.makeMove(msgFromGraphics)); // msgToGraphics should contain the result of the operation
//		try {
			g->playTurn(msgFromGraphics);

		

		// return result to graphics		
		//p.sendMessageToGraphics(msgToGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
	return 0;
}