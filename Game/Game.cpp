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
#include <Engine/Framebuffer/Framebuffer.h>
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

void DrawSceneOntoCubeMap(Engine::Graphics::UniformBuffers::DataPerFrame &dataPerFrame, Engine::Graphics::UniformBuffer &cameraBuffer, 
	Engine::Graphics::CubeMap* cubeMap, Engine::Actor &cubeActor, Engine::Graphics::Shader* shader, const unsigned int viewPortWidth, 
	const unsigned int viewPortHeight)
{
	//Convert equirectangular map to cube map
	{
		//Initialize framebuffer
		Engine::Graphics::Framebuffer cubeMapFrameBuffer(viewPortWidth, viewPortHeight, nullptr);
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 views[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		dataPerFrame.projection = projection;
		glViewport(0, 0, viewPortWidth, viewPortHeight);
		cubeMapFrameBuffer.Bind();
		for (unsigned int i = 0; i < 6; i++) {
			dataPerFrame.view = views[i];
			cameraBuffer.Update(&dataPerFrame);
			cubeMapFrameBuffer.AttachTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cubeActor.Draw(shader);
		}
		cubeMapFrameBuffer.UnBind();
		glViewport(0, 0, screenWidth, screenHeight);
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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);	
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//Initialize shaders	
	Engine::Graphics::Shader* lightShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/simpleMesh.vs", "Assets/Shaders/Fragment/light.fs");
	Engine::Graphics::Shader* pbrShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/mesh.vs", "Assets/Shaders/Fragment/pbr.fs");
	Engine::Graphics::Shader* equiShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/equirectangular_to_cubemap.vs", "Assets/Shaders/Fragment/equirectangular_to_cubemap.fs");
	Engine::Graphics::Shader* cubeMapConvolutionShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/equirectangular_to_cubemap.vs", "Assets/Shaders/Fragment/cubemap_convolution.fs");
	Engine::Graphics::Shader* skyboxShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/skybox.vs", "Assets/Shaders/Fragment/skybox.fs");
	Engine::Graphics::Shader* prefilterShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/equirectangular_to_cubemap.vs", "Assets/Shaders/Fragment/prefilter.fs");

	//Initialize uniform buffer
	Engine::Graphics::UniformBuffers::DataPerFrame dataPerFrame;
	Engine::Graphics::UniformBuffer cameraBuffer(Engine::Graphics::UniformBufferType::DataPerFrame, GL_DYNAMIC_DRAW);

	//Initialize mesh
	Engine::Actor* sphere;
	Engine::Utility::ImportModel("Assets/Models/sphere/sphere.fbx", sphere);
	assert(sphere);
	Engine::Graphics::Mesh* sphereMesh = sphere->GetMesh(0);
	delete sphere;

	Engine::Graphics::Mesh* cubeMesh = Engine::Graphics::Mesh::GetCube();	

	//Initialize textures
	Engine::Graphics::Texture2D* equirectangularMap = Engine::Graphics::Texture2D::CreateHDRTexture("Assets/Textures/hdr/Env.hdr");
	equirectangularMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	equirectangularMap->SetTextureWrappingParams(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	Engine::Graphics::CubeMap* cubeMap = Engine::Graphics::CubeMap::CreateCubeMap(512, 512, GL_RGB16F, GL_RGB, GL_FLOAT);
	cubeMap->SetTextureFilteringParams(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	Engine::Graphics::CubeMap* convolutionCubeMap = Engine::Graphics::CubeMap::CreateCubeMap(32, 32, GL_RGB16F, GL_RGB, GL_FLOAT);

	Engine::Graphics::CubeMap* prefilterEnvironmentMap = Engine::Graphics::CubeMap::CreateCubeMap(128, 128, GL_RGB16F, GL_RGB, GL_FLOAT);
	prefilterEnvironmentMap->SetTextureFilteringParams(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	prefilterEnvironmentMap->GenerateMipMaps();

	Engine::Graphics::Texture2D* brdfLookupTexture = Engine::Graphics::Texture2D::CreateTexture(512, 512, GL_RGB16F, GL_RG, GL_FLOAT);
	brdfLookupTexture->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
	brdfLookupTexture->SetTextureWrappingParams(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	
	//Initialize materials
	const glm::vec3 albedo(0.5f, 0.0f, 0.0f);
	const float ao = 1.0f;
	Engine::Graphics::Material* rustedIronMaterial = Engine::Graphics::Material::CreateMaterial();
	{
		Engine::Graphics::Texture2D* albedoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/albedo.png");
		albedoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		albedoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* aoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/ao.png");
		aoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		aoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* metallicMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/metallic.png");
		metallicMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		metallicMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* normalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/normal.png");
		normalMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		normalMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* roughnessMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/rusted_iron/roughness.png");
		roughnessMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		roughnessMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		rustedIronMaterial->SetAlbedoMap(albedoMap);
		rustedIronMaterial->SetAmbientOcclusionMap(aoMap);
		rustedIronMaterial->SetMetallicMap(metallicMap);
		rustedIronMaterial->SetNormalMap(normalMap);
		rustedIronMaterial->SetRoughnessMap(roughnessMap);

		Engine::Graphics::Texture::DestroyTexture(albedoMap);
		Engine::Graphics::Texture::DestroyTexture(aoMap);
		Engine::Graphics::Texture::DestroyTexture(metallicMap);
		Engine::Graphics::Texture::DestroyTexture(normalMap);
		Engine::Graphics::Texture::DestroyTexture(roughnessMap);
	}

	Engine::Graphics::Material* bathroomTileMaterial = Engine::Graphics::Material::CreateMaterial();
	{
		Engine::Graphics::Texture2D* albedoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/bathroom_tile/albedo.png");
		albedoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		albedoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* aoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/bathroom_tile/ao.png");
		aoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		aoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* metallicMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/bathroom_tile/metallic.psd");
		metallicMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		metallicMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* normalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/bathroom_tile/normal.png");
		normalMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		normalMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		bathroomTileMaterial->SetAlbedoMap(albedoMap);
		bathroomTileMaterial->SetAmbientOcclusionMap(aoMap);
		bathroomTileMaterial->SetMetallicMap(metallicMap);
		bathroomTileMaterial->SetNormalMap(normalMap);
		bathroomTileMaterial->SetRoughness(0.001f);

		Engine::Graphics::Texture::DestroyTexture(albedoMap);
		Engine::Graphics::Texture::DestroyTexture(aoMap);
		Engine::Graphics::Texture::DestroyTexture(metallicMap);
		Engine::Graphics::Texture::DestroyTexture(normalMap);
	}

	Engine::Graphics::Material* goldMaterial = Engine::Graphics::Material::CreateMaterial();
	{
		Engine::Graphics::Texture2D* albedoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/gold/albedo.png");
		albedoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		albedoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* aoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/gold/ao.png");
		aoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		aoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* metallicMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/gold/metallic.png");
		metallicMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		metallicMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* normalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/gold/normal.png");
		normalMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		normalMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* roughnessMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/gold/roughness.png");
		roughnessMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		roughnessMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		goldMaterial->SetAlbedoMap(albedoMap);
		goldMaterial->SetAmbientOcclusionMap(aoMap);
		goldMaterial->SetMetallicMap(metallicMap);
		goldMaterial->SetNormalMap(normalMap);
		goldMaterial->SetRoughnessMap(roughnessMap);

		Engine::Graphics::Texture::DestroyTexture(albedoMap);
		Engine::Graphics::Texture::DestroyTexture(aoMap);
		Engine::Graphics::Texture::DestroyTexture(metallicMap);
		Engine::Graphics::Texture::DestroyTexture(normalMap);
		Engine::Graphics::Texture::DestroyTexture(roughnessMap);
	}

	Engine::Graphics::Material* grassMaterial = Engine::Graphics::Material::CreateMaterial();
	{
		Engine::Graphics::Texture2D* albedoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/grass/albedo.png");
		albedoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		albedoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* aoMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/grass/ao.png");
		aoMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		aoMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* normalMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/grass/normal.png");
		normalMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		normalMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* roughnessMap = Engine::Graphics::Texture2D::CreateTexture("Assets/Textures/pbr/grass/roughness.png");
		roughnessMap->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		roughnessMap->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		grassMaterial->SetAlbedoMap(albedoMap);
		grassMaterial->SetAmbientOcclusionMap(aoMap);
		grassMaterial->SetNormalMap(normalMap);
		grassMaterial->SetRoughnessMap(roughnessMap);
		grassMaterial->SetMetalness(0.0f);

		Engine::Graphics::Texture::DestroyTexture(albedoMap);
		Engine::Graphics::Texture::DestroyTexture(aoMap);
		Engine::Graphics::Texture::DestroyTexture(normalMap);
		Engine::Graphics::Texture::DestroyTexture(roughnessMap);
	}
	
	Engine::Graphics::Material* lightMaterial = Engine::Graphics::Material::CreateMaterial(nullptr, nullptr);
	Engine::Graphics::Material* cubeMaterial = Engine::Graphics::Material::CreateMaterial(equirectangularMap, nullptr);
	Engine::Graphics::Material* skyboxMaterial = Engine::Graphics::Material::CreateMaterial(cubeMap, nullptr);

	//Initialize actors
	Engine::Actor rustedIron(sphereMesh, rustedIronMaterial);
	rustedIron.transform.position.x = -3.0f;
	rustedIron.transform.position.z = -5.0f;

	Engine::Actor gold(sphereMesh, goldMaterial);
	gold.transform.position.x = -1.0f;
	gold.transform.position.z = -5.0f;

	Engine::Actor bathroomTile(sphereMesh, bathroomTileMaterial);
	bathroomTile.transform.position.x = 1.0f;
	bathroomTile.transform.position.z = -5.0f;

	Engine::Actor grass(sphereMesh, grassMaterial);
	grass.transform.position.x = 3.0f;
	grass.transform.position.z = -5.0f;

	Engine::Actor cubeActor(cubeMesh, cubeMaterial);
	Engine::Actor skybox(cubeMesh, skyboxMaterial);

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
	for (int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++) {
		lightActors.push_back(new Engine::Actor(sphereMesh, lightMaterial));
	}	

	std::vector<Engine::Lighting::PointLight*> pointLights;
	for (int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++) {
		pointLights.push_back(new Engine::Lighting::PointLight(lightColors[i] * 0.05f, lightColors[i], lightColors[i], lightActors[i], attennuation));
		pointLights[i]->SetPosition(lightPositions[i]);
		pointLights[i]->ShowMesh(false);
	}

	//Equirectangular to cubemap conversion
	{
		DrawSceneOntoCubeMap(dataPerFrame, cameraBuffer, cubeMap, cubeActor, equiShader, 512, 512);
	}
	
	cubeMap->GenerateMipMaps();
	cubeMaterial->SetDiffuseTexture(cubeMap);

	//Cubemap convolution
	{		
		DrawSceneOntoCubeMap(dataPerFrame, cameraBuffer, convolutionCubeMap, cubeActor, cubeMapConvolutionShader, 32, 32);
	}	

	//Prefiltering the environment map
	{
		Engine::Graphics::Framebuffer prefilterFrameBuffer(128, 128, nullptr);
		unsigned int maxMipmapLevels = 5;		
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 views[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		dataPerFrame.projection = projection;
		prefilterFrameBuffer.Bind();
		prefilterShader->Use();
		prefilterShader->SetInt("environmentMap", 0);
		cubeMap->Bind(0);
		for (unsigned int mip = 0; mip < maxMipmapLevels; mip++) {
			//Resize framebuffer according to mipmap level
			unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			prefilterFrameBuffer.Resize(mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);
			float roughness = mip / (float)(maxMipmapLevels - 1);			
			prefilterShader->SetFloat("currentRoughness", roughness);

			//Draw onto cubemap
			for (unsigned int i = 0; i < 6; i++) {
				dataPerFrame.view = views[i];
				cameraBuffer.Update(&dataPerFrame);
				prefilterFrameBuffer.AttachTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterEnvironmentMap, mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				cubeMesh->Draw();
			}
		}
		prefilterFrameBuffer.UnBind();
	}

	//Create brdf convolution look up texture
	{
		Engine::Graphics::Framebuffer brdfFrameBuffer(512, 512, brdfLookupTexture);
		Engine::Graphics::Mesh* planeMesh = Engine::Graphics::Mesh::GetPlane();
		Engine::Graphics::Shader* brdfConvolutionShader = Engine::Graphics::Shader::CreateShader("Assets/Shaders/Vertex/brdf_convolution.vs", "Assets/Shaders/Fragment/brdf_convolution.fs");

		brdfFrameBuffer.Bind();
		glViewport(0, 0, 512, 512);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			brdfConvolutionShader->Use();
			planeMesh->Draw();
		}
		brdfFrameBuffer.UnBind();

		Engine::Graphics::Mesh::DestroyMesh(planeMesh);
		Engine::Graphics::Shader::DestroyShader(brdfConvolutionShader);
	}

	//Cleaning up resources that will no longer be used
	{
		Engine::Graphics::Shader::DestroyShader(equiShader);
		Engine::Graphics::Shader::DestroyShader(cubeMapConvolutionShader);
		Engine::Graphics::Texture2D::DestroyTexture(equirectangularMap);
		Engine::Graphics::Material::DestroyMaterial(cubeMaterial);
		Engine::Graphics::Shader::DestroyShader(prefilterShader);
	}
		
	//Set IBL textures in sphere materials
	{
		rustedIronMaterial->SetIrradianceMap(convolutionCubeMap);
		rustedIronMaterial->SetPrefilterMap(prefilterEnvironmentMap);
		rustedIronMaterial->SetBRDFLUT(brdfLookupTexture);

		bathroomTileMaterial->SetIrradianceMap(convolutionCubeMap);
		bathroomTileMaterial->SetPrefilterMap(prefilterEnvironmentMap);
		bathroomTileMaterial->SetBRDFLUT(brdfLookupTexture);

		goldMaterial->SetIrradianceMap(convolutionCubeMap);
		goldMaterial->SetPrefilterMap(prefilterEnvironmentMap);
		goldMaterial->SetBRDFLUT(brdfLookupTexture);

		grassMaterial->SetIrradianceMap(convolutionCubeMap);
		grassMaterial->SetPrefilterMap(prefilterEnvironmentMap);
		grassMaterial->SetBRDFLUT(brdfLookupTexture);
	}

	//Load model
	Engine::Actor* cerberus = nullptr;
	Engine::Utility::ImportModel("Assets/Models/Cerberus/Cerberus_LP.FBX", cerberus);
	assert(cerberus);
	cerberus->transform.scale = glm::vec3(0.1f);
	cerberus->transform.position = glm::vec3(0.0f,0.0f,-20.0f);
	{
		//Load PBR textures and set them in the material
		Engine::Graphics::Material* material = cerberus->GetMaterial(0);
		Engine::Graphics::Texture2D* albedo = Engine::Graphics::Texture2D::CreateTexture("Assets/Models/Cerberus/Cerberus_A.tga");
		albedo->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		albedo->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* metallic = Engine::Graphics::Texture2D::CreateTexture("Assets/Models/Cerberus/Cerberus_M.tga");
		metallic->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		metallic->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* normal = Engine::Graphics::Texture2D::CreateTexture("Assets/Models/Cerberus/Cerberus_N.tga");
		normal->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		normal->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		Engine::Graphics::Texture2D* roughness = Engine::Graphics::Texture2D::CreateTexture("Assets/Models/Cerberus/Cerberus_R.tga");
		roughness->SetTextureFilteringParams(GL_LINEAR, GL_LINEAR);
		roughness->SetTextureWrappingParams(GL_REPEAT, GL_REPEAT, GL_REPEAT);

		material->SetAlbedoMap(albedo);
		material->SetMetallicMap(metallic);
		material->SetNormalMap(normal);
		material->SetRoughnessMap(roughness);
		material->SetIrradianceMap(convolutionCubeMap);
		material->SetPrefilterMap(prefilterEnvironmentMap);
		material->SetBRDFLUT(brdfLookupTexture);
		material->SetAmbientOcclusion(ao);

		Engine::Graphics::Material::DestroyMaterial(material);
		Engine::Graphics::Texture2D::DestroyTexture(albedo);
		Engine::Graphics::Texture2D::DestroyTexture(metallic);
		Engine::Graphics::Texture2D::DestroyTexture(normal);
		Engine::Graphics::Texture2D::DestroyTexture(roughness);
	}

	glViewport(0, 0, screenWidth, screenHeight);
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

		for (int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++) {
			dataPerFrame.pointLights[i].isActive = 1.0f;
			dataPerFrame.pointLights[i].lightData.ambient = glm::vec4(pointLights[i]->ambient, 1.0f);
			dataPerFrame.pointLights[i].lightData.diffuse = glm::vec4(pointLights[i]->diffuse, 1.0f);
			dataPerFrame.pointLights[i].lightData.specular = glm::vec4(pointLights[i]->specular, 1.0f);
			dataPerFrame.pointLights[i].linear = pointLights[i]->GetAttenuation().linear;
			dataPerFrame.pointLights[i].quadratic = pointLights[i]->GetAttenuation().quadratic;
			dataPerFrame.pointLights[i].position = glm::vec4(pointLights[i]->GetPosition(), 1.0f);
		}		

		cameraBuffer.Update(&dataPerFrame);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		{
			rustedIron.Draw(pbrShader);
			bathroomTile.Draw(pbrShader);
			gold.Draw(pbrShader);
			grass.Draw(pbrShader);
		}

		{
			cerberus->Draw(pbrShader);
		}

		for (int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++) {
			pointLights[i]->Draw(lightShader);
		}
		
		{
			glDepthFunc(GL_LEQUAL);
			skybox.Draw(skyboxShader);
			glDepthFunc(GL_LESS);
		}

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	//Cleanup
	for (int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++) {
		delete lightActors[i];
		delete pointLights[i];
	}
	lightActors.clear();
	pointLights.clear();
		
	Engine::Graphics::Material::DestroyMaterial(lightMaterial);
	Engine::Graphics::Shader::DestroyShader(lightShader);
	Engine::Graphics::Shader::DestroyShader(pbrShader);
	Engine::Graphics::Mesh::DestroyMesh(sphereMesh);
	Engine::Graphics::Mesh::DestroyMesh(cubeMesh);		
	Engine::Graphics::Texture::DestroyTexture(cubeMap);
	Engine::Graphics::Shader::DestroyShader(skyboxShader);
	Engine::Graphics::Material::DestroyMaterial(skyboxMaterial);
	Engine::Graphics::Texture::DestroyTexture(convolutionCubeMap);
	Engine::Graphics::Texture::DestroyTexture(prefilterEnvironmentMap);
	Engine::Graphics::Texture::DestroyTexture(brdfLookupTexture);
	Engine::Graphics::Material::DestroyMaterial(rustedIronMaterial);
	Engine::Graphics::Material::DestroyMaterial(bathroomTileMaterial);
	Engine::Graphics::Material::DestroyMaterial(goldMaterial);
	Engine::Graphics::Material::DestroyMaterial(grassMaterial);

	delete cerberus;

	glfwTerminate();

	return 0;
}