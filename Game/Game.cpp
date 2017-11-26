#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Engine/Shader/Shader.h>
#include <Engine/Sprite/Sprite.h>
#include <Engine/Mesh/Mesh.h>
#include <Engine/Texture/Texture.h>
#include <Engine/Camera/Camera.h>

float screenWidth = 800.0f;
float screenHeight = 600.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400.0f, lastY = 300.0f;
float pitch = 0.0f, yaw = 0.0f;
bool firstMouse = true;
Engine::Graphics::Camera camera(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
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
	glfwSetCursorPosCallback(window, mouse_callback);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	Engine::Graphics::VertexFormat::Mesh meshVertexData[] = {
		// positions			// colors			// texture coords
		//Back face
		-0.5f, -0.5f, -0.5f,	0.0f,0.0f,0.0f,		1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,0.0f,0.0f,		0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,0.0f,0.0f,		0.0f, 1.0f,		
		-0.5f,  0.5f, -0.5f,	0.0f,0.0f,0.0f,		1.0f, 1.0f,

		//Front face
		-0.5f, -0.5f,  0.5f,	0.0f,0.0f,0.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,0.0f,0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,0.0f,0.0f,		1.0f, 1.0f,		
		-0.5f,  0.5f,  0.5f,	0.0f,0.0f,0.0f,		0.0f, 1.0f,

		//Left face
		-0.5f,  0.5f,  0.5f,	0.0f,0.0f,0.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,0.0f,0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,0.0f,0.0f,		0.0f, 0.0f,		
		-0.5f, -0.5f,  0.5f,	0.0f,0.0f,0.0f,		1.0f, 0.0f,

		//Right face
		0.5f,  0.5f,  0.5f,		0.0f,0.0f,0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,0.0f,0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f,0.0f,0.0f,		1.0f, 0.0f,		
		0.5f, -0.5f,  0.5f,		0.0f,0.0f,0.0f,		0.0f, 0.0f,

		//Bottom face
		-0.5f, -0.5f, -0.5f,	0.0f,0.0f,0.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,0.0f,0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,0.0f,0.0f,		1.0f, 1.0f,		
		-0.5f, -0.5f,  0.5f,	0.0f,0.0f,0.0f,		0.0f, 1.0f,

		//Top face
		-0.5f,  0.5f, -0.5f,	0.0f,0.0f,0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,0.0f,0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,0.0f,0.0f,		1.0f, 0.0f,		
		-0.5f,  0.5f,  0.5f,	0.0f,0.0f,0.0f,		0.0f, 0.0f	
	};

	//index data
	uint32_t indices[] = {
		0, 2, 1, 0, 3, 2,
		4, 5, 6, 4, 6, 7,
		10, 11, 8, 10, 8, 9,
		15, 14, 13, 15, 13, 12,
		16, 17, 18, 16, 18, 19,
		23, 22, 21, 23, 21, 20
	};

	float width = 1.0f;
	float height = 1.0f;

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

	Engine::Graphics::Mesh* meshArray[10];

	//Initialize meshes
	for (unsigned int i = 0; i < 10; i++)
	{
		Engine::Graphics::Mesh* mesh = Engine::Graphics::Mesh::CreateMesh(meshVertexData, sizeof(meshVertexData) / sizeof(Engine::Graphics::VertexFormat::Mesh), indices, sizeof(indices) / sizeof(uint32_t));
		mesh->SetTexture1("Assets/Textures/container.jpg");
		mesh->SetTexture2("Assets/Textures/awesomeface.png", 0.2f);
		mesh->transform.position = cubePositions[i];
		float angle = 20.0f * i;
		mesh->transform.RotateDegrees(angle, glm::vec3(1.0f, 0.3f, 0.5f));
		meshArray[i] = mesh;
	}

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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		//draw
		for (unsigned int i = 0; i < 10; i++) {
			meshArray[i]->Draw(&camera);
		}

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	for (unsigned int i = 0; i < 10; i++) {
		Engine::Graphics::Mesh::DestroyMesh(meshArray[i]);
	}	

	glfwTerminate();
	return 0;
}