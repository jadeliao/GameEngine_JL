#ifndef SERVER_H
#define SERVER_H

#include "User.h"

class Server :public User {

protected:
	SOCKET ListenSocket;
	SOCKET ClientSocket;
public:
	Server();
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime) override;
};

#endif