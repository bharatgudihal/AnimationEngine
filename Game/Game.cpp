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
#include <Engine/Lighting/DirectionalLight/DirectionalLight.h>
#include <Engine/Lighting/PointLight/PointLight.h>
#include <Engine/Lighting/SpotLight/SpotLight.h>
#include <Engine/Material/Material.h>
#include <Engine/UniformBuffer/UniformBuffer.h>
#include <Engine/UniformBuffer/UniformBuffers.h>
#include <Engine/Lighting/Attenuation.h>
#include <Engine/Utility/ModelImporter.h>
#include <vector>

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

	const unsigned int numberOfPointLights = 4;

	//Initialize meshes	
	Engine::Graphics::Mesh* cube = Engine::Graphics::Mesh::GetCube(1.0f, 0.5f, 0.31f);
	Engine::Graphics::Mesh* lightingCube = Engine::Graphics::Mesh::GetCube();

	//Initialize shaders
	Engine::Graphics::Shader cubeShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/mesh.fs");
	Engine::Graphics::Shader lightShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/light.fs");

	//Initialize textures
	Engine::Graphics::Texture* diffuseTexture = Engine::Graphics::Texture::CreateTexture("Assets/Lighting_Maps/Diffuse/container2.png", 0);
	Engine::Graphics::Texture* specularTexture = Engine::Graphics::Texture::CreateTexture("Assets/Lighting_Maps/Specular/container2_specular.png", 1);

	//Initialize Material
	const float shininess = 32.0f;
	Engine::Graphics::Material cubeMaterial(diffuseTexture, specularTexture, shininess);

	//Initialize actors
	const uint8_t numberOfCubes = 10;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::vector<Engine::Graphics::Mesh*> meshes;
	meshes.push_back(cube);
	std::vector<Engine::Graphics::Material*> materials;
	materials.push_back(&cubeMaterial);

	Engine::Actor cubes[] = {
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials),
		Engine::Actor(meshes, materials)
	};
	
	for (uint8_t i = 0; i < numberOfCubes; i++) {
		cubes[i].transform.position = cubePositions[i];
		float angle = 20.0f * i;
		cubes[i].transform.RotateDegrees(angle, glm::vec3(1.0f, 0.3f, 0.5f));
	}

	std::vector<Engine::Graphics::Mesh*> lightMeshes;
	lightMeshes.push_back(lightingCube);
	std::vector<Engine::Graphics::Material*> lightMaterials;
	
	Engine::Actor directionalLightActor(lightMeshes, lightMaterials);
	directionalLightActor.transform.scale = glm::vec3(0.2f);
	directionalLightActor.transform.position = glm::vec3(1.2f, 1.0f, 2.0f);

	Engine::Actor pointLightActors[]{
		Engine::Actor(lightMeshes, lightMaterials),
		Engine::Actor(lightMeshes, lightMaterials),
		Engine::Actor(lightMeshes, lightMaterials),
		Engine::Actor(lightMeshes, lightMaterials)
	};

	for (unsigned int i = 0; i < numberOfPointLights; i++) {
		pointLightActors[i].transform.scale = glm::vec3(0.2f);
	}

	Engine::Actor spotLightActor(lightMeshes, lightMaterials);
	spotLightActor.transform.scale = glm::vec3(0.2f);
	spotLightActor.transform.position = glm::vec3(1.2f, 1.0f, 2.0f);

	//Initialize lights	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
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

	Engine::Lighting::PointLight pointLights[]{
		Engine::Lighting::PointLight(ambient, diffuse, specular, &pointLightActors[0], attenuation),
		Engine::Lighting::PointLight(ambient, diffuse, specular, &pointLightActors[1], attenuation),
		Engine::Lighting::PointLight(ambient, diffuse, specular, &pointLightActors[2], attenuation),
		Engine::Lighting::PointLight(ambient, diffuse, specular, &pointLightActors[3], attenuation)
	};
	
	for (unsigned int i = 0; i < numberOfPointLights; i++) {
		pointLights[i].SetPosition(pointLightPositions[i]);
		pointLights[i].ShowMesh(true);
	}

	Engine::Lighting::SpotLight spotLight(ambient, diffuse, specular, &spotLightActor, lightDirection, glm::radians(12.5f), glm::radians(outerCutOff));
	
	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);
	
	Engine::Actor* model = nullptr;

	Engine::Utility::ImportModel("Assets/nanosuit/nanosuit.obj", model);

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
		spotLight.SetPosition(camera.transform.position);
		spotLight.SetDirection(camera.transform.forward);
		
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
				
		//Point light data
		for (unsigned int i = 0; i < numberOfPointLights; i++) {
			dataPerFrame.pointLights[i].isActive = 1.0f;
			dataPerFrame.pointLights[i].lightData.ambient = glm::vec4(pointLights[i].ambient, 1.0f);
			dataPerFrame.pointLights[i].lightData.diffuse = glm::vec4(pointLights[i].diffuse, 1.0f);
			dataPerFrame.pointLights[i].lightData.specular = glm::vec4(pointLights[i].specular, 1.0f);
			dataPerFrame.pointLights[i].linear = pointLights[i].GetAttenuation().linear;
			dataPerFrame.pointLights[i].quadratic = pointLights[i].GetAttenuation().quadratic;
			dataPerFrame.pointLights[i].position = glm::vec4(pointLights[i].GetPosition(), 1.0f);
		}

		//Spot light data
		dataPerFrame.spotLight.isActive = 1.0f;
		dataPerFrame.spotLight.direction = glm::vec4(spotLight.GetDirection(), 0.0f);
		dataPerFrame.spotLight.position = glm::vec4(spotLight.GetPosition(), 1.0f);
		dataPerFrame.spotLight.innerCutOff = glm::cos(spotLight.GetInnerCutOff());
		dataPerFrame.spotLight.outerCutOff = glm::cos(spotLight.GetOuterCutOff());
		dataPerFrame.spotLight.lightData.ambient = glm::vec4(spotLight.ambient, 1.0f);
		dataPerFrame.spotLight.lightData.diffuse = glm::vec4(spotLight.diffuse, 1.0f);
		dataPerFrame.spotLight.lightData.specular = glm::vec4(spotLight.specular, 1.0f);

		cameraBuffer.Update(&dataPerFrame);

		//draw all actors
		for (uint8_t i = 0; i < numberOfCubes; i++) {
			cubes[i].Draw(&cubeShader);
		}

		model->Draw(&cubeShader);
				
		directionalLight.Draw(&lightShader);

		for (unsigned int i = 0; i < numberOfPointLights; i++) {			
			pointLights[i].Draw(&lightShader);
		}
		spotLightActor.Draw(&lightShader);


		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	Engine::Graphics::Mesh::DestroyMesh(cube);
	Engine::Graphics::Mesh::DestroyMesh(lightingCube);
	Engine::Graphics::Texture::DestroyTexture(diffuseTexture);
	Engine::Graphics::Texture::DestroyTexture(specularTexture);
	delete model;

	glfwTerminate();
	return 0;
}
