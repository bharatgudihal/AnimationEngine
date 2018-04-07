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
bool useParallax = false;
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		useParallax = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		useParallax = false;
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
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	const bool useGamma = true;

	//Initialize meshes
	Engine::Graphics::Mesh* cubeMesh = Engine::Graphics::Mesh::GetCube();
	Engine::Graphics::Mesh* planeMesh = Engine::Graphics::Mesh::GetPlane();

	//Initialize textures	
	Engine::Graphics::Texture2D* woodTexture = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/wood.png", useGamma);
	woodTexture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	woodTexture->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, 0);

	//Initialize shaders
	Engine::Graphics::Shader* meshShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/mesh.fs");
	Engine::Graphics::Shader* lightShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/light.fs");
	Engine::Graphics::Shader* hdrShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/hdr.vs", "Assets/Shaders/Fragment/hdr.fs");

	//Initialize materials
	glm::vec3 ambient(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 specular(1.0f, 1.0f, 1.0f);

	Engine::Graphics::Material* cubeMaterial = Engine::Graphics::Material::CreateMaterial(woodTexture, nullptr, diffuse, specular);
	Engine::Graphics::Material* planeMaterial = Engine::Graphics::Material::CreateMaterial(nullptr, nullptr, diffuse, specular);

	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);

	//Initialize actors
	Engine::Actor cube(cubeMesh, cubeMaterial);
	cube.transform.scale = glm::vec3(2.5f, 2.5f, 27.5f);
	cube.transform.position.z = 10.0f;

	Engine::Actor plane(planeMesh, planeMaterial);
	plane.transform.scale = glm::vec3(2.0f);

	//Initialize lights
	Engine::Lighting::Attenuation attennuation;
	attennuation.linear = 1.0f;
	attennuation.quadratic = 1.0f;

	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 27.0f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	std::vector<Engine::Actor*> lightActors;
	lightActors.push_back(new Engine::Actor(cubeMesh, cubeMaterial));
	lightActors.push_back(new Engine::Actor(cubeMesh, cubeMaterial));
	lightActors.push_back(new Engine::Actor(cubeMesh, cubeMaterial));
	lightActors.push_back(new Engine::Actor(cubeMesh, cubeMaterial));

	std::vector<Engine::Lighting::PointLight*> pointLights;
	for (int i = 0; i < 4; i++) {
		pointLights.push_back(new Engine::Lighting::PointLight(lightColors[i], lightColors[i], lightColors[i], lightActors[i], attennuation));
		pointLights[i]->SetPosition(lightPositions[i]);
		pointLights[i]->ShowMesh(false);
	}

	//Setup render texture
	Engine::Graphics::RenderTexture frameBuffer(screenWidth, screenHeight, GL_RGB16F, GL_RGBA);
	

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
		dataPerFrame.gamma = useGamma ? 2.2f: 1.0f;

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

		//Draw to frame buffer
		{
			frameBuffer.Bind();

			//Clear buffers			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube.Draw(meshShader);

			frameBuffer.UnBind();
		}

		//Draw frame buffer on plane
		{
			//Clear buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			hdrShader->Use();
			hdrShader->SetInt("hdrRender", 0);
			hdrShader->SetFloat("exposure", 1.0f);
			frameBuffer.GetTexture()->Bind(0);
			plane.Draw(hdrShader);
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
	Engine::Graphics::Mesh::DestroyMesh(cubeMesh);
	Engine::Graphics::Shader::DestroyShader(meshShader);
	Engine::Graphics::Material::DestroyMaterial(cubeMaterial);
	Engine::Graphics::Shader::DestroyShader(lightShader);
	Engine::Graphics::Texture::DestroyTexture(woodTexture);
	Engine::Graphics::Mesh::DestroyMesh(planeMesh);
	Engine::Graphics::Shader::DestroyShader(hdrShader);
	Engine::Graphics::Material::DestroyMaterial(planeMaterial);

	for (int i = 0; i < 4; i++) {
		delete lightActors[i];
		delete pointLights[i];
	}
	lightActors.clear();
	pointLights.clear();

	glfwTerminate();

	return 0;
}
