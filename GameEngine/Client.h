#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"

class Client : public User{
protected:
	SOCKET ConnectSocket;
public:
	Client();
	bool OnCreate() override;
	void OnDestroy() override;
	bool Send(const char* actorName_, std::shared_ptr<Actor>) override;
	bool Receive() override;
	void Update(const float deltaTime) override;
};

#endif

