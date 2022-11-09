#ifndef USER_H
#define USER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include "QMath.h"
#include "Quaternion.h"
#include "VMath.h"
#include "Vector.h"
#include "TransformComponent.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27000"
#define STATUS_READ 0x1
#define STATUS_WRITE 0x2
#define STATUS_EXCEPT 0x4
using namespace MATH;

class TransformComponent;

class User {
protected:
	int iResult;
	struct addrinfo *result;
	struct addrinfo hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	Vec3 recvVec;

	void processSendData(Vec3 pos, Quaternion orientation_);
	int getStatus(const SOCKET socket_, int check_);

public:
	virtual ~User(){}
	virtual bool OnCreate() = 0;
	virtual bool Send(std::shared_ptr<TransformComponent>) = 0;
	virtual void OnDestroy() = 0;
	virtual bool Receive() = 0;
	virtual void Update(const float deltaTime) = 0;
	struct addrinfo getHints() { return hints; }
	int getResult() { return iResult; }
	bool processRecvData();
	Vec3 getRecvPos() { return recvVec; }
};

#endif

