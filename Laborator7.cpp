#include "Laborator7.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}


void Tema2::generatePlatforms() {
	for (int lane = 0; lane < maxLanes; lane++) {
		platforms.insert({ lane, *(new std::vector<Platform *>) });
		for (int platform = 0; platform < maxPlatforms; platform++) {
			if (rand() % 5 == 1) {
				platforms[lane].push_back(new Platform(lane, glm::vec3(0), "white", "white", false));
			}
		}
	}
}

void Tema2::generateCollectables() {
	for (int lane = 0; lane < maxLanes; lane++) {
		collectables.insert({ lane, *(new std::vector<Collectable *>) });
		for (int platform = 0; platform < maxPlatforms; platform++) {
			if (rand() % 5 == 1) {
				collectables[lane].push_back(new Collectable(lane, glm::vec3(0)));
			}
		}
	}
}

void Tema2::generateObstacles() {
	for (int lane = 0; lane < maxLanes; lane++) {
		obstacles.insert({ lane, *(new std::vector<Obstacle*>) });
		for (int platform = 0; platform < maxPlatforms; platform++) {
			if (rand() % 100 == 1) {
				obstacles[lane].push_back(new Obstacle(lane, glm::vec3(0)));
			}
		}
	}
}

void Tema2::generateDecoratives() {
	for (int lane = 0; lane < maxLanes; lane++) {
		decoratives.insert({ lane, *(new std::vector<Decorative*>) });
		for (int platform = 0; platform < maxPlatforms; platform++) {
			decoratives[lane].push_back(new Decorative(lane, glm::vec3(0)));
		}
	}
}


void Tema2::generateTextures() {
	Texture2D* texture;
	
	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/background.jpg").c_str(), GL_REPEAT);
	textures["background"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/pink_snow.jpg").c_str(), GL_REPEAT);
	textures["snow_ball"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/snow.jpg").c_str(), GL_REPEAT);
	textures["snow"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/yellow_ice.jpg").c_str(), GL_REPEAT);
	textures["yellow_ice"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/green_ice.jpg").c_str(), GL_REPEAT);
	textures["green_ice"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/red_fire.png").c_str(), GL_REPEAT);
	textures["red_fire"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/ice.jpg").c_str(), GL_REPEAT);
	textures["ice"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/orange_ice.jpg").c_str(), GL_REPEAT);
	textures["orange_ice"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/snow_ball.jpg").c_str(), GL_REPEAT);
	textures["white"] = texture;

	texture = new Texture2D();
	texture->Load2D((texturesFolder + "/present.jpg").c_str(), GL_REPEAT);
	textures["present"] = texture;
}

void Tema2::loadMeshes() {
	// Cream o sfera pentru a afisa jucatorul si alte elemente vizuale
	Mesh* mesh = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// Folosit pentru a desena platformele
	mesh = new Mesh("plane");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// Folosit pentru a desena skybox-ul (tema 3) + coloane, obstacole, etc.
	mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Shader* shader = new Shader("ShaderLab7");
	shader->AddShader("Source/Laboratoare/Laborator7/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Laboratoare/Laborator7/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

void Tema2::Init()
{
	generateTextures();
	generateCollectables();
	generateDecoratives();
	generatePlatforms();
	generateObstacles();
	
	player.resetPlayer();
}

void Tema2::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{

}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));


	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int loc_time = glGetUniformLocation(shader->program, "Time");
	glUniform1f(loc_time, (float)Engine::GetElapsedTime());


	if (texture != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}





void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_A)) {
		player.position.x += player.speed / 7;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		player.position.x -= player.speed / 7;
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		if (player.speed < 1) {
			player.speed += 33 * deltaTime / 500;
		}
	}
	else if (window->KeyHold(GLFW_KEY_S)) {
		if (player.speed > player.position.z / 5 / 2) {
			player.speed -= 44 * deltaTime / 500;
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	switch (key)
	{
	case GLFW_KEY_F:
		camera = 1;
		break;
	case GLFW_KEY_T:
		camera = 2;
		break;
	}

	if (key == GLFW_KEY_SPACE && abs(player.position.y - 1.05) < 0.04) {
		player.jump = 1;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
