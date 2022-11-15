#include "UserInterface.h"

UserInterface::UserInterface():window(nullptr) {

}

UserInterface::~UserInterface(){

}

void UserInterface::OnDestroy(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

bool UserInterface::OnCreate(Window* window_) {
	window = window_;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	if (!ImGui_ImplSDL2_InitForOpenGL(window_->getWindow(), window_->getContext()) ||
		!ImGui_ImplOpenGL3_Init("#version 450")) {
		Debug::FatalError("Failed to initialize UI object", __FILE__, __LINE__);
		return false;
	}

	return true;
}


void UserInterface::Update(const float deltaTime_){

}

void UserInterface::Render() const{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void UserInterface::HandleEvents(){
}
