#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Engine/Shader/Shader.h>
#include <Engine/Sprite/Sprite.h>
#include <Engine/Texture/Texture.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, 800, 600);
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Build and compile shader program
	Engine::Graphics::Shader shader("Assets/Shaders/shader.vs", "Assets/Shaders/shader.fs");
	
	//vertex data
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	//index data
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//Initialize sprite
	Engine::Graphics::Sprite* sprite = Engine::Graphics::Sprite::CreateSprite(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

	//Initialize texture
	Engine::Graphics::Texture* texture1 = Engine::Graphics::Texture::CreateTexture("Assets/Textures/container.jpg",0);
	Engine::Graphics::Texture* texture2 = Engine::Graphics::Texture::CreateTexture("Assets/Textures/awesomeface.png",1);

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);
	shader.SetFloat("blendRatio", 0.2f);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		//Input
		{
			processInput(window);
		}
		
		//Clear color
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		
		
		// bind textures on corresponding texture units
		texture1->Bind();
		texture2->Bind();

		//draw triangle
		shader.Use();		
		sprite->Draw();

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	Engine::Graphics::Sprite::DestroySprite(sprite);
	Engine::Graphics::Texture::DestroyTexture(texture1);
	Engine::Graphics::Texture::DestroyTexture(texture2);

	glfwTerminate();
	return 0;
}