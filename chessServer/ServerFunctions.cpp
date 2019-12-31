#include "ServerFunctions.hpp"
#include "ChessProtocolFunctions.hpp"

#define WAIT_MSG "wait"
#define CONNECT_MSG "connect"
#define DISCONNECT_MSG "disconnect"
#define MAX_SIZE 1024

void ServerFunctions::createNewConnection(sf::TcpListener& listener, sf::TcpSocket* client1, sf::TcpSocket* client2)
{
	if (listener.accept(*client1) != sf::Socket::Done)
		throw std::exception("Can not accept connection");

	if (client1->send(WAIT_MSG, strlen(WAIT_MSG) + 1) != sf::Socket::Done)  //Send waiting msg to first client
		throw(std::exception("Client 1 is disconnected"));

	if (listener.accept(*client2) != sf::Socket::Done)
		throw(std::exception("Client 2 is disconnected"));


	if (client1->send(CONNECT_MSG, strlen(CONNECT_MSG) + 1) != sf::Socket::Done)  //Send connect msg to first client
		throw(std::exception("Client 1 is disconnected"));

	if (sendBoards(client1, client2) != sf::Socket::Done)
	{
		throw(std::exception("One of the Clients is disconnected"));
	}
}

int ServerFunctions::sendBoards(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	const char* white_gui_board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
	const char* white_array_board = "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";
	const char* black_gui_board = "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr0";
	const char* black_array_board = "RNBKQBNRPPPPPPPP################################pppppppprnbKqbnr";

	if (sock1->send(white_gui_board, strlen(white_gui_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock2);
		return sf::Socket::Error;
	}
	if (sock1->send(white_array_board, strlen(white_array_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock2);
		return sf::Socket::Error;
	}
	if (sock2->send(black_gui_board, strlen(black_gui_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock1);
		return sf::Socket::Error;
	}
	if (sock2->send(black_array_board, strlen(black_array_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock1);
		return sf::Socket::Error;
	}

	return sf::Socket::Done;
}

void ServerFunctions::sendDisconnectMsg(sf::TcpSocket* sock)
{
	sock->send(DISCONNECT_MSG, strlen(DISCONNECT_MSG) + 1);
}

void ServerFunctions::forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2) //, std::mutex sock1_mu, std::mutex sock2_mu)
{
	char data[MAX_SIZE];
	std::size_t received;
	while (true)
	{
		if (sock1->receive(data, MAX_SIZE, received) != sf::Socket::Done)
		{
			ServerFunctions::sendDisconnectMsg(sock2);
			break;
		}
		else
		{
			if (!ChessProtocol::isMsgCmd(data))
				ChessProtocol::convertMoveCmd(data);
			
			if (sock2->send(data, strlen(data) + 1) != sf::Socket::Done)
			{
				ServerFunctions::sendDisconnectMsg(sock1);
				break;
			}
		}
	}
}

void ServerFunctions::matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	//std::mutex sock1_mu, sock2_mu;
	std::thread(ServerFunctions::forward_msg, sock1, sock2).detach();
	std::thread(ServerFunctions::forward_msg, sock2, sock1).detach();

}
