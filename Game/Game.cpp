#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
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
#include <vector>

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>
#include <crtdbg.h>

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

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	const unsigned int numberOfPointLights = 4;

	//Initialize meshes	
	Engine::Graphics::Mesh* cube = Engine::Graphics::Mesh::GetCube(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* lightingCube = Engine::Graphics::Mesh::GetCube();

	//Initialize shaders
	Engine::Graphics::Shader* modelShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/mesh.fs");
	Engine::Graphics::Shader* lightShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/light.fs");	
	
	//Initialize lights	

	std::vector<Engine::Graphics::Mesh*> lightMeshes;
	lightMeshes.push_back(lightingCube);
	std::vector<Engine::Graphics::Material*> lightMaterials;

	Engine::Actor directionalLightActor(lightMeshes, lightMaterials);
	directionalLightActor.transform.scale = glm::vec3(0.2f);
	directionalLightActor.transform.position = glm::vec3(1.2f, 1.0f, 2.0f);

	Engine::Actor spotLightActor(lightMeshes, lightMaterials);
	spotLightActor.transform.scale = glm::vec3(0.2f);
	spotLightActor.transform.position = glm::vec3(1.2f, 1.0f, 2.0f);

	Engine::Actor pointLightActor1(lightMeshes, lightMaterials);
	pointLightActor1.transform.scale = glm::vec3(0.2f);

	Engine::Actor pointLightActor2(lightMeshes, lightMaterials);
	pointLightActor2.transform.scale = glm::vec3(0.2f);
		
	glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 specular(1.0f, 1.0f, 1.0f);
	
	glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
	Engine::Lighting::Attenuation attenuation;
	attenuation.linear = 0.09f;
	attenuation.quadratic = 0.032f;
	float innerCutOff = 12.5f;
	float outerCutOff = 15.0f;

	Engine::Lighting::DirectionalLight directionalLight(ambient, diffuse, specular, &directionalLightActor, lightDirection);

	Engine::Lighting::PointLight pointLight1(ambient, diffuse, specular, &pointLightActor1, attenuation);
	//pointLight1.ShowMesh(true);
	pointLight1.SetPosition(glm::vec3(-1.0f, 1.0f, 1.0f));

	Engine::Lighting::PointLight pointLight2(ambient, diffuse, specular, &pointLightActor2, attenuation);
	//pointLight2.ShowMesh(true);
	pointLight2.SetPosition(glm::vec3(1.0f, 1.0f, 1.0f));
	
	//Engine::Lighting::SpotLight spotLight(ambient, diffuse, specular, &spotLightActor, lightDirection, glm::radians(12.5f), glm::radians(outerCutOff));
	
	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);
	
	Engine::Actor* model = nullptr;
	Engine::Utility::ImportModel("Assets/nanosuit/nanosuit.obj", model);
	model->transform.scale = glm::vec3(0.2f);
	model->transform.position.y = -1.0f;

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

		//Update spot light position direction
		//spotLight.SetPosition(camera.transform.position);
		//spotLight.SetDirection(camera.transform.forward);
		
		//Clear color
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}		
				
		dataPerFrame.view = camera.GetViewMatrix();
		dataPerFrame.projection = camera.GetProjectionMatrix();
		dataPerFrame.viewPos = glm::vec4(camera.transform.position, 1.0f);

		//Direction light data
		dataPerFrame.directionalLight.isActive = glm::vec4(1.0f);
		dataPerFrame.directionalLight.direction = glm::vec4(directionalLight.GetLightDirection(), 0.0f);
		dataPerFrame.directionalLight.lightData.ambient = glm::vec4(directionalLight.ambient, 1.0f);		
		dataPerFrame.directionalLight.lightData.diffuse = glm::vec4(directionalLight.diffuse, 1.0f);
		dataPerFrame.directionalLight.lightData.specular = glm::vec4(directionalLight.specular, 1.0f);

		dataPerFrame.pointLights[0].isActive = true;
		dataPerFrame.pointLights[0].lightData.ambient = glm::vec4(pointLight1.ambient, 1.0f);
		dataPerFrame.pointLights[0].lightData.diffuse = glm::vec4(pointLight1.diffuse, 1.0f);
		dataPerFrame.pointLights[0].lightData.specular = glm::vec4(pointLight1.specular, 1.0f);
		dataPerFrame.pointLights[0].linear = pointLight1.GetAttenuation().linear;
		dataPerFrame.pointLights[0].quadratic = pointLight1.GetAttenuation().quadratic;
		dataPerFrame.pointLights[0].position = glm::vec4(pointLight1.GetPosition(), 1.0f);

		dataPerFrame.pointLights[1].isActive = true;
		dataPerFrame.pointLights[1].lightData.ambient = glm::vec4(pointLight2.ambient, 1.0f);
		dataPerFrame.pointLights[1].lightData.diffuse = glm::vec4(pointLight2.diffuse, 1.0f);
		dataPerFrame.pointLights[1].lightData.specular = glm::vec4(pointLight2.specular, 1.0f);
		dataPerFrame.pointLights[1].linear = pointLight2.GetAttenuation().linear;
		dataPerFrame.pointLights[1].quadratic = pointLight2.GetAttenuation().quadratic;
		dataPerFrame.pointLights[1].position = glm::vec4(pointLight2.GetPosition(), 1.0f);

		//Spot light data
		/*dataPerFrame.spotLight.isActive = 1.0f;
		dataPerFrame.spotLight.direction = glm::vec4(spotLight.GetDirection(), 0.0f);
		dataPerFrame.spotLight.position = glm::vec4(spotLight.GetPosition(), 1.0f);
		dataPerFrame.spotLight.innerCutOff = glm::cos(spotLight.GetInnerCutOff());
		dataPerFrame.spotLight.outerCutOff = glm::cos(spotLight.GetOuterCutOff());
		dataPerFrame.spotLight.lightData.ambient = glm::vec4(spotLight.ambient, 1.0f);
		dataPerFrame.spotLight.lightData.diffuse = glm::vec4(spotLight.diffuse, 1.0f);
		dataPerFrame.spotLight.lightData.specular = glm::vec4(spotLight.specular, 1.0f);*/

		cameraBuffer.Update(&dataPerFrame);

		//draw all actors
		model->Draw(modelShader);				
		directionalLight.Draw(lightShader);
		pointLight1.Draw(lightShader);
		pointLight2.Draw(lightShader);

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	lightMeshes.clear();
	Engine::Graphics::Mesh::DestroyMesh(cube);
	Engine::Graphics::Mesh::DestroyMesh(lightingCube);	
	Engine::Graphics::Shader::DestroyShader(modelShader);
	Engine::Graphics::Shader::DestroyShader(lightShader);
	delete model;

	glfwTerminate();

	return 0;
}
