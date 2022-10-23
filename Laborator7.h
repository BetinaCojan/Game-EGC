#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <vector>
#include <unordered_map>

class Player {
public:
	glm::vec3 position;
	float score;
	float speed;
	float jump;

	Player() {}

	void resetPlayer() {
		this->position = glm::vec3(0);
		this->score = 0;
		this->speed = 0;
	}

	void modifySpeed(float value) {
		speed += value;
	}
};

class Platform {
public:
	int lane;
	glm::vec3 position;
	std::string color;
	std::string texture;
	bool isVisible;

	Platform(int lane, glm::vec3 position, std::string color, std::string texture, bool isVisible) {
		this->lane = lane;
		this->position = position;
		this->color = color;
		this->texture = texture;
		this->isVisible = isVisible;
	}
};

class Decorative {
public:
	glm::vec3 position;

	Decorative(glm::vec3 position) {
		this->position = position;
	}
};

class Collectable {
public:
	int lane;
	glm::vec3 position;

	Collectable(int lane, glm::vec3 position) {
		this->lane = lane;
		this->position = position;
	}
};

class Obstacle {
public:
	int lane;
	glm::vec3 position;
	
	Obstacle(int lane, glm::vec3 position) {
		this->lane = lane;
		this->position = position;
	}
};

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;
	private:
		void FrameStart() override;
		
		// Lab base functions
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture = NULL);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;


		// Game constans
		const int maxLanes = 3;
		const int maxPlatforms = 500;
		const int platformLength = 5;
		const int platformWidth = 2;

		// Player
		Player player;

		// Platforms
		unordered_map<int, std::vector<Platform*>> platforms;
		void generatePlatforms();

		// Decoratives
		std::vector<Decorative *> decoratives;
		void generateDecoratives();

		// Collectables
		unordered_map<int, std::vector<Collectable*>> collectables;
		void generateCollectables();

		// Obstacles
		unordered_map<int, std::vector<Collectable*>> obstacles;
		void generateObstacles();

		// Textures
		const string texturesFolder = "Source/Laboratoare/Laborator7/Textures";
		unordered_map<std::string, Texture2D *> textures;
		void generateTextures();

		// Meshes
		void Tema2::loadMeshes();

		int camera;

		GLenum polygonMode = GL_FILL;
};
