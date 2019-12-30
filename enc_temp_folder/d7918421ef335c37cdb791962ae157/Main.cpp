#include <iostream>
#include <SFML/Network.hpp>
#include <future>
#include <thread>
#include <vector>

#define WAIT_MSG "Wait"
#define CONNECT_MSG "Connect"


void forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	char data[10240];
	std::size_t received;
	while (true)
	{
		if (sock1->receive(data, 10240, received) != sf::Socket::Done) 
		{

		}
		std::cout << "Received: " << std::string(data) << std::endl;
		if (sock2->send(data, strlen(data)) != sf::Socket::Done){

		}
		std::cout << "Sending: " << std::string(data) << std::endl;
	}
}

void matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	while (true) 
	{
		std::thread(forward_msg, sock1, sock2).detach();
		std::thread(forward_msg, sock2, sock1).detach();
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
		
		if (client1->send(CONNECT_MSG, strlen(CONNECT_MSG)) != sf::Socket::Done)   //Send connect msg to second client
			continue;

		match = new std::thread(matchThread, client1, client2);
		match->detach();
		threads.push_back(match);
	}

	return 0;
}

