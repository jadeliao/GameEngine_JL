#ifndef USER_H
#define USER_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>
#include "QMath.h"
#include "Quaternion.h"
#include "VMath.h"
#include "Vector.h"
#include "TransformComponent.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27000"
#define STATUS_READ 0x1
#define STATUS_WRITE 0x2
#define STATUS_EXCEPT 0x4

using namespace MATH;

class Actor;
class TransformComponent;

struct ActorData {
	const char* actorName;
	Vec3 actorPos;
};

enum UserType {
	NONE,
	SERVER,
	CLIENT
};

class User {
protected:
	UserType userType;
	int iResult;
	struct addrinfo *result;
	struct addrinfo hints;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	Vec3 recvVec;
	ActorData* recvActor;
	bool isConnect;
	std::queue<std::string> recvBufList;
	std::queue<ActorData*> recvList;

	void processSendData(const char* actorName_, Actor* actor_);
	int getStatus(const SOCKET socket_, int check_);

public:
	
	User(UserType userType_):isConnect(false), userType(userType_) {}
	virtual ~User(){}
	virtual bool OnCreate() = 0;
	virtual bool Send(const char* actorName_, std::shared_ptr<Actor>) = 0;
	virtual void OnDestroy() = 0;
	virtual bool Receive() = 0;
	virtual void Update(const float deltaTime) = 0;
	struct addrinfo getHints() { return hints; }
	int getResult() { return iResult; }
	UserType getUserType() { return userType; }
	bool processRecvData();
	Vec3 getRecvPos() { return recvVec; }
	ActorData* getActorData() {

		if (!recvBufList.empty()) {
			//Process data
			if (!processRecvData()) {
				//std::cout << "Client Incorrect Recveive Data: " << recvbuf << std::endl;
				return nullptr;
			}
		}
		if (!recvList.empty()) {
			ActorData* data_ = recvList.front();
			recvList.pop();
			return data_;
		}

		//if (recvActor) return *recvActor; 
		return {};
	}

	bool isConnected() { return isConnect; }
};

#endif

