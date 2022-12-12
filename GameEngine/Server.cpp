#include "Server.h"

Server::Server():User(UserType::SERVER) {
	ClientSocket = INVALID_SOCKET;
	ListenSocket = INVALID_SOCKET;
}

bool Server::OnCreate() {

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		system("pause");
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		system("pause");
		return false;
	}


	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return false;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return false;
	}

	printf("Waiting for connection request...\n");
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		system("pause");
		return false;
	}
	else {
		printf("Client Found!\n");
		//Make it non-blocking
		isConnect = true;
		u_long mode = 1;
		iResult = ioctlsocket(ClientSocket, FIONBIO, &mode);
		if (iResult != NO_ERROR) {
			printf("ioctlsocket failed with error: %ld\n", iResult);
		}
	}
	// No longer need server socket
	closesocket(ListenSocket);
	iResult = 1;
	return true;
}

void Server::OnDestroy() {
	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		system("pause");
	}
	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

bool Server::Send(const char* actorName_, Ref<Actor> actor_) {
	
	// Send an initial buffer
	processSendData(actorName_, actor_.get());
	iResult = send(ClientSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		system("pause");
		return false;
	}
	else {
		//Clear the text buffer
		memset(sendbuf, 0, sizeof(sendbuf));
		//std::cout << "Server Message sent\n";
		return true;
	}
	memset(sendbuf, 0, sizeof(sendbuf));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

bool Server::Receive() {
	if (getStatus(ClientSocket, STATUS_READ) == 1) {
		//std::cout << "Server Running...\n";
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			//if (!processRecvData()) {
			//	std::cout << "Client Incorrect Recveive Data: " << recvbuf << std::endl;
			//	return false;
			//}
			recvBufList.push(std::string(recvbuf));
			//printf("Client received: %d\n", iResult);
			std::cout << "Client received: " << recvBufList.front() << std::endl;
			memset(recvbuf, 0, sizeof(recvbuf));
			return true;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	}
	return false;
}

void Server::Update(const float deltaTime) {

}