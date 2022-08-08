#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
 
int main() {
	sf::IpAddress localIp = sf::IpAddress::getLocalAddress();
	sf::IpAddress connectIp;
	unsigned int port;
	sf::TcpSocket socket;
	sf::Packet packet;
	bool isMyTurn;
	std::string myUserName;
	std::string friendUserName;
	std::string message;
	std::string choose;

	std::cout << "Enter your name: ";
	std::cin >> myUserName;
	std::cout << std::endl;

	do {
		std::cout << "Connect or make(connect/make): ";
		std::cin >> choose;
		std::cout << std::endl;
	} while (choose != "connect" && choose != "make");

	if (choose == "connect") {
		std::cout << "Enter IP: ";
		std::cin >> connectIp;
		std::cout << "Enter PORT: ";
		std::cin >> port;
		std::cout << std::endl;

		socket.connect(connectIp, port);

		packet << myUserName;
		socket.send(packet);
		packet.clear();

		socket.receive(packet);
		packet >> friendUserName;
		packet.clear();
	} else {
		std::cout << "Your IP: " << localIp << std::endl;
		std::cout << "Enter PORT: ";
		std::cin >> port;

		sf::TcpListener listener;
		listener.listen(port);
		listener.accept(socket);

		socket.receive(packet);
		packet >> friendUserName;
		packet.clear();

		packet << myUserName;
		socket.send(packet);
		packet.clear();

		isMyTurn = 1;
	}

	while (true) {
		if (isMyTurn) {
			std::cout << myUserName << ": ";
			getline(std::cin, message);
			if (message != "") {
				packet << message;
				socket.send(packet);
				packet.clear();

				isMyTurn = 0;
			}
		}

		else {
			socket.receive(packet);
			packet >> message; 
			std::cout << friendUserName << ": " << message << std::endl;
			packet.clear();

			isMyTurn = 1;
		}
	}

	return 0;
}
