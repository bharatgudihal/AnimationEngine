#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Engine/Shader/Shader.h>
#include <Engine/Sprite/Sprite.h>
#include <Engine/Mesh/Mesh.h>
#include <Engine/Texture/Texture.h>
#include <Engine/Camera/Camera.h>
#include <Engine/Actor/Actor.h>
#include <Engine/Math/Math.h>
#include <Engine/Lighting/SimpleLight.h>
#include <Engine/Material/Material.h>

float screenWidth = 800.0f;
float screenHeight = 600.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400.0f, lastY = 300.0f;
float pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;
Engine::Graphics::Camera camera(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = 2.5f * deltaTime;
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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	

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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Initialize meshes	
	Engine::Graphics::Mesh* cube = Engine::Graphics::Mesh::GetCube(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* lightingCube = Engine::Graphics::Mesh::GetCube();

	//Initialize shaders
	Engine::Graphics::Shader cubeShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/mesh.fs");
	Engine::Graphics::Shader lightShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/light.fs");
	
	//Initialize textures
	Engine::Graphics::Texture* cubeTexture1 = Engine::Graphics::Texture::CreateTexture("Assets/Textures/container.jpg", 0);
	Engine::Graphics::Texture* cubeTexture2 = Engine::Graphics::Texture::CreateTexture("Assets/Textures/awesomeface.png", 1);

	//Initialize actor
	Engine::Actor cubeActor(cube, &cubeShader);	
	Engine::Actor lightActor(lightingCube, &lightShader);
	lightActor.transform.scale = glm::vec3(0.2f);
	lightActor.transform.position = glm::vec3(1.2f, 1.0f, 2.0f);

	//Initialize light
	glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 specular(1.0f, 1.0f, 1.0f);
	Engine::Lighting::SimpleLight simpleLight(ambient, diffuse, specular, &lightActor);
	simpleLight.ShowMesh(true);

	//Initialize Material
	Engine::Graphics::Material cubeMaterial;
	cubeMaterial.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	cubeMaterial.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	cubeMaterial.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeMaterial.shininess = 32.0f;

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		//Time calculations
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Input
		{
			processInput(window);
		}
		
		//Clear color
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//draw
		simpleLight.Apply(&cubeShader);
		cubeShader.Use();
		cubeShader.SetMatrix("model", Engine::Math::CalculateTransform(cubeActor.transform));
		cubeShader.SetMatrix("view", camera.GetViewMatrix());
		cubeShader.SetMatrix("projection", camera.GetProjectionMatrix());
		cubeShader.SetVector("viewPos", camera.transform.position);
		cubeShader.SetVector("material.ambient", cubeMaterial.ambient);
		cubeShader.SetVector("material.diffuse", cubeMaterial.diffuse);
		cubeShader.SetVector("material.specular", cubeMaterial.specular);
		cubeShader.SetFloat("material.shininess", cubeMaterial.shininess);
		lightShader.Use();
		lightShader.SetMatrix("model", Engine::Math::CalculateTransform(lightActor.transform));
		lightShader.SetMatrix("view", camera.GetViewMatrix());
		lightShader.SetMatrix("projection", camera.GetProjectionMatrix());
		
		cubeActor.Draw();
		simpleLight.Draw();

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	Engine::Graphics::Mesh::DestroyMesh(cube);
	Engine::Graphics::Mesh::DestroyMesh(lightingCube);
	Engine::Graphics::Texture::DestroyTexture(cubeTexture1);
	Engine::Graphics::Texture::DestroyTexture(cubeTexture2);

	glfwTerminate();
	return 0;
}