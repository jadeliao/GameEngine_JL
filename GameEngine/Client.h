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
	void Update(const float deltaTime) override;
};

#endif

