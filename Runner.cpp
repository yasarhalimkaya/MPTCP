#include "Connection.hpp"

int main(int argc, char* argv[]) {

	if (argc != 3) {
		std::cout << "Specify the destination address and port" << std::endl;
		return 0;
	}

	Connection connection(argv[1], atoi(argv[2]));

	char message[100] = "Message";

	connection.send(message);

	strcpy(message, "terminate");

	connection.send(message);

	return 0;
}
