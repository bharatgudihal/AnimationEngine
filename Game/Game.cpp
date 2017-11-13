#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Engine/Shader/Shader.h>
#include <Engine/Sprite/Sprite.h>

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
	Engine::Shader shader("Shaders/shader.vs", "Shaders/shader.fs");
	
	//vertex data
	float vertices[] = {
		// positions		// colors
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// bottom right
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// bottom left
		0.0f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// top
	};

	//index data
	unsigned int indices[] = {
		0,2,1
	};	

	//Initialize sprite
	Engine::Sprite* sprite = Engine::Sprite::CreateSprite(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

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
		
		//draw triangle
		shader.Use();		
		sprite->Draw();

		//Call events and swap buffers
		{
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	Engine::Sprite::DestroySprite(sprite);

	glfwTerminate();
	return 0;
}