#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Engine/Shader/Shader.h>
#include <Engine/Sprite/Sprite.h>
#include <Engine/Mesh/Mesh.h>
#include <Engine/Texture/Texture.h>
#include <Engine/Camera/Camera.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
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

	float screenWidth = 800.0f;
	float screenHeight = 600.0f;

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

	//Initialize sprite
	Engine::Graphics::Sprite* sprite = Engine::Graphics::Sprite::CreateSprite(0.5f,0.5f);
	sprite->SetTexture1("Assets/Textures/container.jpg");
	sprite->SetTexture2("Assets/Textures/awesomeface.png",0.2f);
	sprite->transform.position = Engine::Graphics::Math::Vector3(0.75f, -0.75f, 0.0f);

	//Initialize mesh
	Engine::Graphics::Mesh* mesh = Engine::Graphics::Mesh::CreateMesh(meshVertexData, sizeof(meshVertexData) / sizeof(Engine::Graphics::VertexFormat::Mesh), indices, sizeof(indices) / sizeof(uint32_t));
	mesh->SetTexture1("Assets/Textures/container.jpg");
	mesh->SetTexture2("Assets/Textures/awesomeface.png", 0.2f);
	mesh->transform.position = Engine::Graphics::Math::Vector3(0.0f, 0.0f, 0.0f);

	//Initialize camera
	Engine::Graphics::Camera camera(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);
	camera.transform.position.z = -3.0f;

	// Render loop
	while (!glfwWindowShouldClose(window)) {
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
		//sprite->Draw();
		
		
		mesh->transform.Rotate((float)glfwGetTime() * glm::radians(50.0f), Engine::Graphics::Math::Vector3(0.5f, 1.0f, 0.0f));
		mesh->Draw(&camera);

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	Engine::Graphics::Sprite::DestroySprite(sprite);
	Engine::Graphics::Mesh::DestroyMesh(mesh);

	glfwTerminate();
	return 0;
}