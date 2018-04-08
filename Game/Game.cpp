#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine/Shader/Shader.h>
#include <Engine/Mesh/Mesh.h>
#include <Engine/Texture/Texture2D.h>
#include <Engine/Camera/Camera.h>
#include <Engine/Actor/Actor.h>
#include <Engine/Math/Math.h>
#include <Engine/Lighting/DirectionalLight/DirectionalLight.h>
#include <Engine/Lighting/PointLight/PointLight.h>
#include <Engine/Lighting/SpotLight/SpotLight.h>
#include <Engine/Material/Material.h>
#include <Engine/UniformBuffer/UniformBuffer.h>
#include <Engine/UniformBuffer/UniformBuffers.h>
#include <Engine/Lighting/Attenuation.h>
#include <Engine/Utility/ModelImporter.h>
#include <Engine/RenderTexture/RenderTexture.h>
#include <Engine/Texture/CubeMap.h>
#include <vector>
#include <iostream>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>

const unsigned int screenWidth = 1280;
const unsigned int screenHeight = 720;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = screenWidth / 2.0f, lastY = screenHeight / 2.0f;
float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;
Engine::Graphics::Camera camera(45.0f, screenWidth / (float)screenHeight, 0.1f, 100.0f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.transform.position += camera.transform.forward * cameraSpeed;
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.transform.position -= camera.transform.forward * cameraSpeed;
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.transform.position -= glm::vec3(glm::cross(camera.transform.forward,camera.transform.up)) * cameraSpeed;
	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.transform.position += glm::vec3(glm::cross(camera.transform.forward, camera.transform.up)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		
	}
		
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos);
	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.transform.forward = glm::normalize(front);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (camera.FOV >= 1.0f && camera.FOV <= 45.0f) {
		camera.FOV -= static_cast<float>(yoffset);
	}
	if (camera.FOV < 1.0f) {
		camera.FOV = 1.0f;
	}
	if (camera.FOV > 45.0f) {
		camera.FOV = 45.0f;
	}
}

int main(int argc, char* argv[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//Set OpenGL properties
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	const unsigned int nrRows = 7;
	const unsigned int nrColumns = 7;
	const float spacing = 2.5;

	//Initialize shaders	
	Engine::Graphics::Shader* lightShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/light.fs");
	Engine::Graphics::Shader* pbrShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/pbr.fs");

	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);

	//Initialize mesh
	Engine::Actor* sphere;
	Engine::Utility::ImportModel("Assets/Models/sphere/sphere.fbx", sphere);
	assert(sphere);
	Engine::Graphics::Mesh* sphereMesh = sphere->GetMesh(0);
	delete sphere;

	//Initialize textures
	Engine::Graphics::Texture2D* albedoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/albedo.png");
	Engine::Graphics::Texture2D* aoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/ao.png");
	Engine::Graphics::Texture2D* metallicMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/metallic.png");
	Engine::Graphics::Texture2D* normalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/normal.png");
	Engine::Graphics::Texture2D* roughnessMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/roughness.png");

	//Initialize materials
	Engine::Graphics::Material* sphereMaterial = Engine::Graphics::Material::CreateMaterial(nullptr, nullptr);
	sphereMaterial->SetAlbedoMap(albedoMap);
	sphereMaterial->SetAmbientOcclusionMap(aoMap);
	sphereMaterial->SetMetallicMap(metallicMap);
	sphereMaterial->SetNormalMap(normalMap);
	sphereMaterial->SetRoughnessMap(roughnessMap);

	Engine::Graphics::Material* lightMaterial = Engine::Graphics::Material::CreateMaterial(nullptr, nullptr);

	//Initialize actors
	Engine::Actor* spheres[nrRows][nrColumns];
	for (unsigned int i = 0; i < nrRows; i++) {
		for (unsigned int j = 0; j < nrColumns; j++) {
			spheres[i][j] = new Engine::Actor(sphereMesh, sphereMaterial);
			spheres[i][j]->transform.position = 
				glm::vec3((float)(j - (nrColumns / 2.0f)) * spacing,
				(float)(i - (nrRows / 2.0f)) * spacing,
				0.0f);
		}
	}

	//Initialize lights
	Engine::Lighting::Attenuation attennuation;
	attennuation.linear = 1.0f;
	attennuation.quadratic = 1.0f;

	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	std::vector<Engine::Actor*> lightActors;
	lightActors.push_back(new Engine::Actor(sphereMesh, lightMaterial));
	lightActors.push_back(new Engine::Actor(sphereMesh, lightMaterial));
	lightActors.push_back(new Engine::Actor(sphereMesh, lightMaterial));
	lightActors.push_back(new Engine::Actor(sphereMesh, lightMaterial));

	std::vector<Engine::Lighting::PointLight*> pointLights;
	for (int i = 0; i < 4; i++) {
		pointLights.push_back(new Engine::Lighting::PointLight(lightColors[i] * 0.05f, lightColors[i], lightColors[i], lightActors[i], attennuation));
		pointLights[i]->SetPosition(lightPositions[i]);
		pointLights[i]->ShowMesh(false);
	}

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//Time calculations
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input
		{
			processInput(window);
		}

		dataPerFrame.view = camera.GetViewMatrix();
		dataPerFrame.projection = camera.GetProjectionMatrix();
		dataPerFrame.viewPos = glm::vec4(camera.transform.position, 1.0f);
		dataPerFrame.gamma = 1.0f;

		for (int i = 0; i < 4; i++) {
			dataPerFrame.pointLights[i].isActive = 1.0f;
			dataPerFrame.pointLights[i].lightData.ambient = glm::vec4(pointLights[i]->ambient, 1.0f);
			dataPerFrame.pointLights[i].lightData.diffuse = glm::vec4(pointLights[i]->diffuse, 1.0f);
			dataPerFrame.pointLights[i].lightData.specular = glm::vec4(pointLights[i]->specular, 1.0f);
			dataPerFrame.pointLights[i].linear = pointLights[i]->GetAttenuation().linear;
			dataPerFrame.pointLights[i].quadratic = pointLights[i]->GetAttenuation().quadratic;
			dataPerFrame.pointLights[i].position = glm::vec4(pointLights[i]->GetPosition(), 1.0f);
		}		

		cameraBuffer.Update(&dataPerFrame);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (unsigned int i = 0; i < nrRows; i++) {
			for (unsigned int j = 0; j < nrColumns; j++) {
				spheres[i][j]->Draw(pbrShader);
			}
		}

		for (int i = 0; i < 4; i++) {
			pointLights[i]->Draw(lightShader);
		}

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	for (unsigned int i = 0; i < nrRows; i++) {
		for (unsigned int j = 0; j < nrColumns; j++) {
			delete spheres[i][j];
		}
	}

	for (int i = 0; i < 4; i++) {
		delete lightActors[i];
		delete pointLights[i];
	}
	lightActors.clear();
	pointLights.clear();

	Engine::Graphics::Material::DestroyMaterial(lightMaterial);
	Engine::Graphics::Shader::DestroyShader(lightShader);
	Engine::Graphics::Shader::DestroyShader(pbrShader);
	Engine::Graphics::Mesh::DestroyMesh(sphereMesh);
	Engine::Graphics::Material::DestroyMaterial(sphereMaterial);
	Engine::Graphics::Texture::DestroyTexture(albedoMap);
	Engine::Graphics::Texture::DestroyTexture(aoMap);
	Engine::Graphics::Texture::DestroyTexture(metallicMap);
	Engine::Graphics::Texture::DestroyTexture(normalMap);
	Engine::Graphics::Texture::DestroyTexture(roughnessMap);	

	glfwTerminate();

	return 0;
}