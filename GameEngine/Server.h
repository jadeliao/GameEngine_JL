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
	bool Send(std::shared_ptr<TransformComponent>) override;
	bool Receive() override;
	void Update(const float deltaTime) override;

};

#endif