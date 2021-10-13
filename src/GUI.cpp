#include "GUI.h"

GUI::GUI(GLFWwindow* window, World* world) :	world_(world), frequency_(0.5), amplitude_(5), lacunarity_(2),
												octaves_(2), numChunks_(3) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsLight();
}

GUI::~GUI() {

}

void GUI::render(Shader* shader) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Generation Menu");
	ImGui::SliderFloat("Frequency", &frequency_, 0.075, 0.6);
	ImGui::SliderFloat("Amplitude", &amplitude_, 0.5, 10.0);
	ImGui::SliderFloat("Lacunarity (for multiple octaves)", &lacunarity_, 2, 5);
	ImGui::SliderInt("Octaves", &octaves_, 1, 8);
	ImGui::SliderInt("Number of Chunks", &numChunks_, 1, 16);
	if (ImGui::SliderFloat3("Light Position", lightPosition_, -500, 500)) {
		GLint light = glGetUniformLocation(shader->getShader(), "lightPos");
		glUniform3f(light, lightPosition_[0], lightPosition_[1], lightPosition_[2]);
	}
	if (ImGui::Button("Generate")) {
		world_->getNoise()->setFrequency(frequency_);
		world_->getNoise()->setOctaves(octaves_);
		world_->getNoise()->setAmplitude(amplitude_);
		world_->getNoise()->setLacunarity(lacunarity_);
		world_->setNumChunks(numChunks_);
		world_->generate();
	}
	if (ImGui::Button("Generate with New Permutation")) {
		world_->getNoise()->setFrequency(frequency_);
		world_->getNoise()->setOctaves(octaves_);
		world_->getNoise()->setAmplitude(amplitude_);
		world_->getNoise()->setLacunarity(lacunarity_);
		world_->setNumChunks(numChunks_);
		world_->getNoise()->newPermutation();
		world_->generate();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}