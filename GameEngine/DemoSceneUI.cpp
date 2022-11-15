#include "DemoSceneUI.h"

void DemoSceneUI::Update(const float deltaTime_){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	bool status = true;
	ImGui::SetNextWindowPos(ImVec2(5.0f, window->getHeight() - 30.0f));
	ImGui::Begin("Another Window", &status, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

}
