#include "Client.h"

Client::Client(): User(UserType::CLIENT) {
	ConnectSocket = INVALID_SOCKET;
}

bool Client::OnCreate() {

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		system("pause");
		return 1;
	}

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
		u_long mode = 1;
		iResult = ioctlsocket(ConnectSocket, FIONBIO, &mode);
		if (iResult != NO_ERROR) {
			printf("ioctlsocket failed with error: %ld\n", iResult);
		}
		printf("Connect to Server!\n");
		isConnect = true;
	}
	iResult = 1;
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

bool Client::Send(const char* actorName_, std::shared_ptr<Actor> actor_) {
	// Send an initial buffer
	processSendData(actorName_, actor_.get());
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		system("pause");
		return false;
	}
	else {
		//Clear the text buffer
		memset(sendbuf, 0, sizeof(sendbuf));
		//std::cout << "Client Message sent\n";
		return true;
	}
	memset(sendbuf, 0, sizeof(sendbuf));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

} 

bool Client::Receive() {

	if (getStatus(ConnectSocket, STATUS_READ) == 1) {
		//std::cout << "Client Receiving...\n";
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			if (!processRecvData()) {
				std::cout << "Client Incorrent Recveive Data: " << recvbuf << std::endl;
				return false;
			}
			//printf("Client received: %d\n", iResult);
			//std::cout << "Client received: " << recvbuf << std::endl;
			return true;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());
	}
	return false;
}

void Client::Update(const float deltaTime) {

}