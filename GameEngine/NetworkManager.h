#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace std;

struct ActorData;
class Actor;
enum UserType;

class NetworkManager {
private:
	static shared_ptr<NetworkManager> _instance;
	WSADATA wsaData;
	class User* user;
	int iResult;
	bool isRunning;
	NetworkManager();

public:
	static shared_ptr<NetworkManager> getInstance();
	~NetworkManager();
	bool Receive();
	bool Initialize();
	void Shutdown();
	bool Send(const char* actorName_, std::shared_ptr<Actor> actor_);
	ActorData* getReceive();
	UserType getUserType();
	bool isConnect();
};


#endif