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

using std::cout;
using std::endl;
using std::string;

void serverListener(sf::TcpSocket* sock, Pipe chatPipe, Pipe changePipe)
{
	char data[10240] = { 0 };
	string msg = "";
	string value = "";
	std::size_t received;
	while (true) {
		sock->receive(data, 10240, received);
		msg = string(data);
		if (msg.find("chat") != string::npos) {
			strcpy(data, msg.c_str());
			cout << "Received from server: " << msg;
			changePipe.sendMessageToGraphics(data);
			cout << "Sent to pipe: " << msg << endl;
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
	srand(time_t(NULL));
	std::string str4gui = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
	std::string str4game ="rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";
	//ayer playerPl(white);
	Pipe p(0);
	Pipe change(1);
	Pipe chat(2);
	bool isConnectP = p.connect();
	bool isConnectChange = change.connect();
	sf::TcpSocket* sock = new sf::TcpSocket();
	sock->connect("127.0.0.1", 5000);
	chat.connect();
	string ans;
	std::thread serverThread(serverListener, sock, chat, change);
	serverThread.detach();
	std::thread chatThread(chatPipeListener, sock, chat);
	chatThread.detach();

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
	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, str4gui.c_str()); // just example...

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();
	Game g(str4game, p, change);

	while (msgFromGraphics != "quit")
	{
		// should handle the string the sent from graphics
		// according the protocol. Ex: e2e4           (move e2 to e4)

		// YOUR CODE
		//strcpy_s(msgToGraphics, player.makeMove(msgFromGraphics)); // msgToGraphics should contain the result of the operation
//		try {
			g.playTurn(msgFromGraphics);

		

		// return result to graphics		
		//p.sendMessageToGraphics(msgToGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
	return 0;
}