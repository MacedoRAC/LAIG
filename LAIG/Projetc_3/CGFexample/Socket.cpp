#include "Socket.h"

Socket::Socket() {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	
	if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
	else
		printf("Client: WSAStartup() is OK.\n");
	
	// Create a socket.
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (m_socket == INVALID_SOCKET) {
		printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		throw exception();
	} else
		printf("Client: socket() is OK.\n");
	
	// Connect to a server.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	
	// Just test using the localhost, you can try other IP address
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(60001);
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Client: connect() - Failed to connect.\n");
		WSACleanup();
		throw exception();
	} else {
		printf("Client: connect() is OK.\n");
		printf("Client: Can start sending and receiving data...\n");
	}

	// Send and receive data.
	printf("Connected\n");
}

string Socket::sendMessage(string message) {
	message += ".\n";
	int len = message.length();
	const char* s = message.c_str();
	int bytesSent = send(m_socket, s, len, 0);
	if(bytesSent == SOCKET_ERROR) {
		throw exception();
		//printf("Client: send() error %ld.\n", WSAGetLastError());
	}
	// Receive Response
	string answer = "";
	int pos = 0;
	while (true) {
		char buffer;
		recv(m_socket, &buffer, 1, 0);
		if (buffer == '\n')
			break;
		answer += buffer;
	}
	return answer;
}

Socket::~Socket() {
	cout << "Asking prolog to quit" << endl;
	char buff[] = "bye.\n";
	sendMessage("bye");
}