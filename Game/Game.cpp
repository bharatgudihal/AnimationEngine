#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <Engine/Shader/Shader.h>
#include <Engine/Mesh/Mesh.h>
#include <Engine/Texture/Texture.h>
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
#include <vector>
#include <iostream>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>

int screenWidth = 800;
int screenHeight = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400.0f, lastY = 300.0f;
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//Set OpenGL properties
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);	

	//Initialize meshes	
	Engine::Graphics::Mesh* cubeMesh = Engine::Graphics::Mesh::GetCube(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* planeMesh = Engine::Graphics::Mesh::GetPlane(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* lightingCube = Engine::Graphics::Mesh::GetCube();	

	//Initialize textures
	Engine::Graphics::Texture* planeTexture = Engine::Graphics::Texture::CreateTexture("Assets/Textures/metal.png");
	planeTexture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	planeTexture->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT);
	Engine::Graphics::Texture* cubeTexture = Engine::Graphics::Texture::CreateTexture("Assets/Textures/container.jpg");
	cubeTexture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	cubeTexture->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT);
	
	//Initialize shaders
	Engine::Graphics::Shader* simpleMeshShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/simpleMesh.fs");
	Engine::Graphics::Shader* outlineShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/outline.fs");
	
	//Initialize materials
	glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 specular(1.0f, 1.0f, 1.0f);
	Engine::Graphics::Material* planeMaterial = Engine::Graphics::Material::CreateMaterial(planeTexture, nullptr, diffuse, specular);	

	Engine::Graphics::Material* cubeMaterial = Engine::Graphics::Material::CreateMaterial(cubeTexture, nullptr, diffuse, specular);	
	
	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);
	
	//Initialize actors
	Engine::Actor plane(planeMesh, planeMaterial);
	plane.transform.scale = glm::vec3(5.0f);
	plane.transform.position.y = -0.5f;

	Engine::Actor cube(cubeMesh, cubeMaterial);

	//Initialize render to texture
	Engine::Graphics::RenderTexture renderTexture(screenWidth, screenHeight, GL_RGB);

	//New plane
	Engine::Graphics::Material* reflectionPlaneMaterial = Engine::Graphics::Material::CreateMaterial(renderTexture.GetTexture(), nullptr, diffuse, specular);
	Engine::Actor reflectionPlane(planeMesh, reflectionPlaneMaterial);
	reflectionPlane.transform.scale = glm::vec3(5.0f);	
	reflectionPlane.transform.position.z = -5.0f;
	reflectionPlane.transform.RotateDegrees(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

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
						
		dataPerFrame.view = camera.GetViewMatrix();
		dataPerFrame.projection = camera.GetProjectionMatrix();
		dataPerFrame.viewPos = glm::vec4(camera.transform.position, 1.0f);

		cameraBuffer.Update(&dataPerFrame);
			
		renderTexture.Bind();

		//Clear buffers
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//Draw cubes
		{
			cube.transform.position = glm::vec3(-1.0f, 0.0f, -1.0f);
			cube.Draw(simpleMeshShader);

			cube.transform.position = glm::vec3(2.0f, 0.0f, 0.0f);
			cube.Draw(simpleMeshShader);
		}

		//draw plane
		{
			plane.Draw(simpleMeshShader);
		}

		renderTexture.UnBind();

		//Clear buffers
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		reflectionPlane.Draw(simpleMeshShader);

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	Engine::Graphics::Mesh::DestroyMesh(cubeMesh);
	Engine::Graphics::Mesh::DestroyMesh(planeMesh);
	Engine::Graphics::Mesh::DestroyMesh(lightingCube);	
	Engine::Graphics::Shader::DestroyShader(simpleMeshShader);
	Engine::Graphics::Shader::DestroyShader(outlineShader);
	Engine::Graphics::Material::DestroyMaterial(planeMaterial);
	Engine::Graphics::Material::DestroyMaterial(cubeMaterial);
	Engine::Graphics::Material::DestroyMaterial(reflectionPlaneMaterial);
	Engine::Graphics::Texture::DestroyTexture(planeTexture);
	Engine::Graphics::Texture::DestroyTexture(cubeTexture);

	glfwTerminate();

	return 0;
}
