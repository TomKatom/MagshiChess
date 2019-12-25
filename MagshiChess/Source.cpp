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

using std::cout;
using std::endl;
using std::string;

void check_pieces()
{
	srand(time_t(NULL));
	std::string str4gui = "r#bq#b#rpppppppp################################PPPPPPPPR#BQ#B#R1";
	std::string str4game = "r#b#qb#rpppppppp################################PPPPPPPPR#B#QB#R";
	//ayer playerPl(white);
	Pipe p;
	bool isConnect = p.connect();

	string ans;
	while (!isConnect)
	{
		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			Sleep(5000);
			isConnect = p.connect();
		}
		else
		{
			p.close();
			return;
		}
	}


	char msgToGraphics[1024];
	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, str4gui.c_str()); // just example...

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();
	Game g(str4game, p);

	while (msgFromGraphics != "quit")
	{
		// should handle the string the sent from graphics
		// according the protocol. Ex: e2e4           (move e2 to e4)

		// YOUR CODE
		//strcpy_s(msgToGraphics, player.makeMove(msgFromGraphics)); // msgToGraphics should contain the result of the operation
		g.playTurn(msgFromGraphics);

		// return result to graphics		
		//p.sendMessageToGraphics(msgToGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
}

int main()
{
	check_pieces();
	/*
	srand(time_t(NULL));

	Player player;
	Pipe p;
	bool isConnect = p.connect();

	string ans;
	while (!isConnect)
	{
		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans == "0")
		{
			cout << "trying connect again.." << endl;
			Sleep(5000);
			isConnect = p.connect();
		}
		else
		{
			p.close();
			return;
		}
	}

	player.setPipe(p);

	char msgToGraphics[1024];
	// msgToGraphics should contain the board string accord the protocol
	// YOUR CODE

	strcpy_s(msgToGraphics, "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR1"); // just example...

	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();

	while (msgFromGraphics != "quit")
	{
		// should handle the string the sent from graphics
		// according the protocol. Ex: e2e4           (move e2 to e4)

		// YOUR CODE
		//strcpy_s(msgToGraphics, player.makeMove(msgFromGraphics)); // msgToGraphics should contain the result of the operation
		player.makeMove(msgFromGraphics);

		// return result to graphics		
		//p.sendMessageToGraphics(msgToGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
	*/
	return 0;
}