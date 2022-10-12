#include "NetworkManager.h"
#include "User.h"
#include "Server.h"
#include "Client.h"

NetworkManager::NetworkManager(): user(nullptr), isRunning(false) {
}

NetworkManager::~NetworkManager() {
	if (user) user->OnDestroy(), delete user;
}

void NetworkManager::Run() {

}

bool NetworkManager::Initialize() {

	//Ask for user type before initializing
	while (!user) {
		char usertype[DEFAULT_BUFLEN] = "";
		cout << "Choose user type (S)Server (C)Client:";
		cin >> usertype;
		//Create Server
		if (strcmp(usertype, "S") == 0) {
			user = new Server();
			cout << "Server Created!\n";
		}
		//Create Client
		else if (strcmp(usertype, "C") == 0) {
			user = new Client();
			cout << "Client Created!\n";
		}
		else {
			cout << "Invalid user type.\n";
		}
	}

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
		return 1;
	}

	//Start connecting user
	if (user->OnCreate()) {
		isRunning = true;
	}
	return isRunning;
}

void NetworkManager::Shutdown() {
}