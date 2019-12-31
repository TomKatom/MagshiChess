#include "ServerFunctions.hpp"

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
			continue;
		
		try {
			ServerFunctions::createNewConnection(listener, client1, client2);
		}
		catch (std::exception & e)
		{
			continue;
		}

		match = new std::thread(ServerFunctions::matchThread, client1, client2);
		match->detach();
		threads.push_back(match);
	}

	return 0;
}

