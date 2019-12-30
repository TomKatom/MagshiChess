#include <iostream>
#include <SFML/Network.hpp>
#include <future>
#include <thread>
#include <vector>

#define WAIT_MSG "wait"
#define CONNECT_MSG "connect"
#define DISCONNECT_MSG "disconnect"
#define MAX_SIZE 1024
#define WHITE_STARTING_BOARD ""

int sendBoards(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	const char* white_gui_board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
	const char* white_array_board = "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";
	const char* black_gui_board = "";
	const char* black_array_board = "";
	int sent_board_code = 0;
	return sent_board_code;
}

void sendDisconnectMsg(sf::TcpSocket* sock)
{
	sock->send(DISCONNECT_MSG, strlen(DISCONNECT_MSG));
}

void forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	char data[MAX_SIZE];
	std::size_t received;
	while (true)
	{
		if (sock1->receive(data, MAX_SIZE, received) != sf::Socket::Done)
		{
			sendDisconnectMsg(sock2);
			throw std::exception("Player1 disconnected");
		}
		else
		{
			std::cout << "Received: " << std::string(data) << std::endl;
			if (sock2->send(data, strlen(data)) != sf::Socket::Done) 
			{
				sendDisconnectMsg(sock1);
				throw std::exception("Player2 disconnected");
			}
			std::cout << "Sending: " << std::string(data) << std::endl;
		}
	}
}

void matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	while (true) 
	{
		try {
			std::thread(forward_msg, sock1, sock2).detach();
			std::thread(forward_msg, sock2, sock1).detach();
		}
		catch (std::exception & e)
		{
			break;
		}
	}
}

int main() 
{
	sf::TcpListener listener;
	std::vector<std::thread*> threads;
	std::thread* match;

	while (true) 
	{

		sf::TcpSocket* client1 = new sf::TcpSocket;
		sf::TcpSocket* client2 = new sf::TcpSocket;
	

		if (listener.listen(5000) != sf::Socket::Done)
		{
			// error...
		}
	
		if (listener.accept(*client1) != sf::Socket::Done)
			continue;  

		if (client1->send(WAIT_MSG, strlen(WAIT_MSG)) != sf::Socket::Done)  //Send waiting msg to first client
			continue;

		if (listener.accept(*client2) != sf::Socket::Done)
			continue;
		

		if (client1->send(CONNECT_MSG, strlen(CONNECT_MSG)) != sf::Socket::Done)  //Send connect msg to first client
			continue;
		
		if (client2->send(CONNECT_MSG, strlen(CONNECT_MSG)) != sf::Socket::Done)   //Send connect msg to second client
		{
			sendDisconnectMsg(client1);
			continue;
		}
			

		sendBoards(client1, client2);

		match = new std::thread(matchThread, client1, client2);
		match->detach();
		threads.push_back(match);
	}

	return 0;
}

