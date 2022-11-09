#ifndef USER_H
#define USER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27000"

class User {
protected:
	int iResult;
	struct addrinfo *result;
	struct addrinfo hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

public:
	virtual ~User(){}
	virtual bool OnCreate();
	virtual bool Send(char sendbuf[]) = 0;
	virtual void OnDestroy() = 0;
	virtual void Run() = 0;
	virtual void Update(const float deltaTime) = 0;
	struct addrinfo getHints() { return hints; }
	int getResult() { return iResult; }
};

#endif

