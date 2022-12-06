#include "NetworkManager.h"
#include "Server.h"
#include "Client.h"
#include "Actor.h"
#include <thread>

shared_ptr<NetworkManager> NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager() : user(nullptr), isRunning(false) {
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

bool NetworkManager::Receive() {

	if (!user) return false;

	if (user->getResult() > 0)
		return user->Receive();
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

	//Detach user oncreate
	std::thread user_create(&User::OnCreate, user);
	user_create.detach();
	//Start connecting user
	//if (user->OnCreate()) {
	//	isRunning = true;
	//}
	isRunning = true;
	return isRunning;
}

void NetworkManager::Shutdown() {
}

bool NetworkManager::Send(const char* actorName_, std::shared_ptr<Actor> actor_){

	if (!user) return false;

	if (!user->Send(actorName_, actor_)) {
		return false;
	}

	return true;
}

ActorData* NetworkManager::getReceive() {
	return user->getActorData();
	//return nullptr;
}

UserType NetworkManager::getUserType(){
	if (user) return user->getUserType();
	return UserType::NONE;
}

bool NetworkManager::isConnect(){
	if (user) {
		return user->isConnected();
	}
	return false;
}

