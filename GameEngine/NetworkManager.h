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
	WSADATA wsaData;
	class User* user;
	int iResult;
	bool isRunning;
public:
	NetworkManager();
	~NetworkManager();
	void Run();
	bool Initialize();
	void Shutdown();
	
};



#endif