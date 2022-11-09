#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

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
	void Run();
	bool Initialize();
	void Shutdown();
	bool Send(char sendbuf[]);
	
};


#endif