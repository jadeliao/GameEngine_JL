#include "NetworkManager.h"
#include "User.h"
#include "Server.h"
#include "Client.h"

shared_ptr<NetworkManager> NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager(): user(nullptr), isRunning(false) {
}

shared_ptr<NetworkManager> NetworkManager::getInstance() {

	if (!_instance.get()) {
		_instance.reset(new NetworkManager);
	}

	return _instance;
}

NetworkManager::~NetworkManager() {
	if (user) user->OnDestroy(), delete user;
}

void NetworkManager::Run() {
	do {
		user->Run();
	} while (user->getResult() > 0);
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

bool NetworkManager::Send(char sendbuf[]){
	if (!user->Send(sendbuf)) {
		return false;
	}
	std::cout << "Data sent\n";
	return true;
}
