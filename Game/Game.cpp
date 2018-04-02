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

	const bool useGamma = false;

	//Initialize meshes	
	Engine::Graphics::Mesh* planeMesh = Engine::Graphics::Mesh::GetPlane(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* cubeMesh = Engine::Graphics::Mesh::GetCube();

	//Initialize textures	
	Engine::Graphics::Texture2D* planeTexture = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/brickwall.jpg", useGamma);
	planeTexture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	planeTexture->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, 0);

	Engine::Graphics::Texture2D* planeNormalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/brickwall_normal.jpg", useGamma);
	planeNormalMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	planeNormalMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, 0);
	
	//Initialize shaders
	Engine::Graphics::Shader* meshShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/mesh.fs");
	Engine::Graphics::Shader* lightShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/light.fs");
	
	//Initialize materials
	glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 specular(1.0f, 1.0f, 1.0f);	

	Engine::Graphics::Material* planeMaterial = Engine::Graphics::Material::CreateMaterial(planeTexture, nullptr, diffuse, specular);
	planeMaterial->SetNormalMap(planeNormalMap);
	Engine::Graphics::Material* cubeMaterial = Engine::Graphics::Material::CreateMaterial(nullptr, nullptr, diffuse, specular);
	
	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);
	
	//Initialize actors
	Engine::Actor plane(planeMesh, planeMaterial);
	plane.transform.scale = glm::vec3(5.0f);
	plane.transform.position.z = -3.0f;
	plane.transform.RotateDegrees(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	//Initialize lights
	Engine::Lighting::Attenuation attennuation;
	attennuation.linear = 0.22f;
	attennuation.quadratic = 0.2f;

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	
	Engine::Actor lightCube(cubeMesh, cubeMaterial);

	Engine::Lighting::PointLight pointLight(lightColor, lightColor, lightColor, &lightCube, attennuation);
	pointLight.SetPosition(glm::vec3(0.5f, 1.0f, 0.3f));

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
		dataPerFrame.gamma = useGamma ? 2.2f : 1.0f;
		
		dataPerFrame.pointLights[0].isActive = 1.0f;
		dataPerFrame.pointLights[0].lightData.ambient = glm::vec4(pointLight.ambient, 1.0f);
		dataPerFrame.pointLights[0].lightData.diffuse = glm::vec4(pointLight.diffuse, 1.0f);
		dataPerFrame.pointLights[0].lightData.specular = glm::vec4(pointLight.specular, 1.0f);
		dataPerFrame.pointLights[0].linear = pointLight.GetAttenuation().linear;
		dataPerFrame.pointLights[0].quadratic = pointLight.GetAttenuation().quadratic;
		dataPerFrame.pointLights[0].position = glm::vec4(pointLight.GetPosition(), 1.0f);

		cameraBuffer.Update(&dataPerFrame);
		
		//Clear buffers
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}	
		
		pointLight.Draw(lightShader);

		//Draw plane
		{
			plane.Draw(meshShader);
		}

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	Engine::Graphics::Mesh::DestroyMesh(cubeMesh);	
	Engine::Graphics::Shader::DestroyShader(meshShader);	
	Engine::Graphics::Texture2D::DestroyTexture(planeTexture);
	Engine::Graphics::Material::DestroyMaterial(cubeMaterial);
	Engine::Graphics::Material::DestroyMaterial(planeMaterial);
	Engine::Graphics::Mesh::DestroyMesh(planeMesh);
	Engine::Graphics::Texture::DestroyTexture(planeNormalMap);
	Engine::Graphics::Shader::DestroyShader(lightShader);

	glfwTerminate();

	return 0;
}
