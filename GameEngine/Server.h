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
	bool Send(const char* actorName_, std::shared_ptr<Actor>) override;
	bool Receive() override;
	void Update(const float deltaTime) override;

};

#endif