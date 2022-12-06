#define _CRTDBG_MAP_ALLOC  
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>  
#include <crtdbg.h>
#include <memory>
#include <thread>
#include <string>
#include "SceneManager.h"
#include "NetworkManager.h"
#include "Debug.h"

int main(int argc, char* args[]) {
	
	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");

	Debug::DebugInit("GameEngineLog.txt");
	{
		std::shared_ptr<NetworkManager> netManager = NetworkManager::getInstance();

		//std::thread net_thread(&NetworkManager::Initialize, netManager);
		//net_thread.detach();
		if (netManager->Initialize()) {
			std::unique_ptr<SceneManager> game = std::make_unique<SceneManager>();
			if (game->Initialize("Game Engine", 1280, 720) == true) {
				game->Run();
			}
		}
	}
	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	exit(0);
}