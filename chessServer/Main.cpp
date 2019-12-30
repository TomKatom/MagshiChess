#include <iostream>
#include <SFML/Network.hpp>
#include <future>
#include <thread>
#include <vector>
/*
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
*/

void forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2) {
	char data[10240];
	std::size_t received;
	while (true) {
		if (sock1->receive(data, 10240, received) != sf::Socket::Done) {

		}
		std::cout << "Received: " << std::string(data) << std::endl;
		if (sock2->send(data, strlen(data)) != sf::Socket::Done){

		}
		std::cout << "Sending: " << std::string(data) << std::endl;
	}
}

void matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2) {
	while (true) {
		std::thread(forward_msg, sock1, sock2).detach();
		std::thread(forward_msg, sock2, sock1).detach();
	}
}
int main() {
	sf::TcpListener listener;
	std::vector<std::thread*> threads;
	std::thread* match;
	while (true) {

		sf::TcpSocket* client1 = new sf::TcpSocket;
		sf::TcpSocket* client2 = new sf::TcpSocket;
	
		if (listener.listen(5000) != sf::Socket::Done)
		{
			// error...
		}
	
		if (listener.accept(*client1) != sf::Socket::Done)
		{
			// error...
		}
		if (listener.accept(*client2) != sf::Socket::Done)
		{
			// error...
		}
		match = new std::thread(matchThread, client1, client2);
		match->detach();
		threads.push_back(match);
	}

	return 0;
}

