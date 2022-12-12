#include "User.h"
#include "Actor.h"

void User::processSendData(const char* actorName_, Actor* actor_){
	memset(sendbuf, 0, sizeof(sendbuf));
	ActorData actorData = {};
	actorData.actorName = actorName_;
	actorData.actorPos = actor_->GetComponent<TransformComponent>()->GetPosition();
	std::string temp(actorName_);
	//Add name
	temp = temp + "\n";
	std::string posStr = std::to_string(actorData.actorPos.x) + "," +
	std::to_string(actorData.actorPos.y) + "," +
	std::to_string(actorData.actorPos.z) + "\n";
	temp = temp + posStr;
	//char *temp = reinterpret_cast<char*>(&actorData);
	//std::cout << "Send " << temp << "\n";
	strcat_s(sendbuf, temp.c_str());
}

//void User::processSendData(Actor* actor_){
//	std::string posStr = std::to_string(pos.x) + "," +
//		std::to_string(pos.y) + "," +
//		std::to_string(pos.z) + "\n";
//	strcat_s(sendbuf, posStr.c_str());
//}

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

	//Grab the first data from the list
	std::string actorData(recvBufList.front());
	recvBufList.pop();
	std::cout << actorData << std::endl;
	//Transform the data into vector
	ActorData* actorData_ = new ActorData;
	int nameIndex = actorData.find("\n");
	char* actorName = new char[DEFAULT_BUFLEN];
	strcpy_s(actorName, DEFAULT_BUFLEN, actorData.substr(0, nameIndex).c_str());
	std::cout << actorName << "\n";
	actorData.erase(0, nameIndex + 1);
	actorData_->actorName = actorName;
	std::string posStr = actorData;
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
	actorData_->actorPos = Vec3(posx, posy, posz);
	recvList.push(actorData_);
	std::cout << "Receive Actor name " << actorData_->actorName;
	actorData_->actorPos.print();
	std::cout << "\n";
		//std::string posStr(recvbuf);
		//int posxIndex = posStr.find(",");
		//if (posStr.length() <= posxIndex) return false;
		//float posx = std::stof(posStr.substr(0, posxIndex));
		//posStr.erase(0, posxIndex+1);
		//int posyIndex = posStr.find(",");
		//if (posStr.length() <= posyIndex) return false;
		//float posy = std::stof(posStr.substr(0, posyIndex));
		//posStr.erase(0, posyIndex+1);
		//int poszIndex = posStr.find("\n");
		//if (posStr.length() <= poszIndex) return false;
		//float posz = std::stof(posStr.substr(0, poszIndex));
		//posStr.erase(0, poszIndex);
		////Clear the text buffer
		//memset(recvbuf, 0, sizeof(recvbuf));
		//recvVec = Vec3(posx, posy, posz);
	//recvActor = reinterpret_cast<ActorData*>(recvbuf);
	//std::cout << "Receive " << recvActor->actorPos << "\n";
	if (!actorData_) return false;

	return true;
}

