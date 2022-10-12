#include "Client.h"

Client::Client() {
	ConnectSocket = INVALID_SOCKET;
}

bool Client::OnCreate() {
	User::OnCreate();

	// Attempt to connect to an address until one succeeds
	for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			system("pause");
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		system("pause");
		return false;
	}
	else {
		printf("Connect to Server!\n");
	}

	return true;
}

void Client::OnDestroy() {
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		system("pause");
	}
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

void Client::Update(const float deltaTime) {

}