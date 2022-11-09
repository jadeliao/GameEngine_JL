#include "User.h"

void User::processSendData(Vec3 pos, Quaternion orientation_){
	std::string posStr = std::to_string(pos.x) + "," +
		std::to_string(pos.y) + "," +
		std::to_string(pos.z) + "\n";
	strcat_s(sendbuf, posStr.c_str());
}

int User::getStatus(const SOCKET socket_, int check_){
	static timeval timeout = { 0,0 }; // 0 sec
	fd_set a = { 1, {socket_} };
	//fd_set* read = ((check_ & 0x1) != 0) ? &a : NULL;
	//fd_set* write = ((check_ & 0x2) != 0) ? &a : NULL;
	//fd_set* except = ((check_ & 0x4) != 0) ? &a : NULL;
	fd_set* read = ((check_ & 0x1) != 0) ? &a : NULL;
	fd_set* write = nullptr;
	fd_set* except = nullptr;
	int result = select(0, &a, write, except, &timeout);
	if (result == SOCKET_ERROR)	{
		result = WSAGetLastError();
	}
	if (result < 0 || result > 3)	{
		printf("select error %d\n", result);
		return SOCKET_ERROR;
	}
	return result;
}

bool User::processRecvData() {
		std::string posStr(recvbuf);
		int posxIndex = posStr.find(",");
		if (posStr.length() <= posxIndex) return false;
		float posx = std::stof(posStr.substr(0, posxIndex));
		posStr.erase(0, posxIndex+1);
		int posyIndex = posStr.find(",");
		if (posStr.length() <= posyIndex) return false;
		float posy = std::stof(posStr.substr(0, posyIndex));
		posStr.erase(0, posyIndex+1);
		int poszIndex = posStr.find("\n");
		if (posStr.length() <= poszIndex) return false;
		float posz = std::stof(posStr.substr(0, poszIndex));
		posStr.erase(0, poszIndex);
		//Clear the text buffer
		memset(recvbuf, 0, sizeof(recvbuf));

		recvVec = Vec3(posx, posy, posz);
		return true;
}

