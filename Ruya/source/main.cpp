#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "MainWindow.h"
#include "Shader.h"

enum class RenderMode {FILL, WIREFRAME};
RenderMode renderMode = RenderMode::FILL;
bool allowRenderModeChange = true;

void processInputs(GLFWwindow* window);
void mainloop(ruya::MainWindow& mainWindow);

int main()
{
	// Hint windowing system the version of opengl used. This code is tested on gtx 1050m which supports
	// opengl 4.6 at the moment.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	ruya::MainWindow mainWindow;
	mainWindow.makeContextCurrent();

	// init functions pointers to opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD (which is responsible for the opengl function pointers)" << std::endl;
		return -1;
	}

	// main loop
	mainloop(mainWindow);

	glfwTerminate(); // clean up all reasources allocated by glfw.
	return 0;
}

std::string readFileContents(const char* fileName)
{
	std::ifstream vertexShaderFile(fileName);
	if (!vertexShaderFile.is_open())
	{
		std::cerr << "Could not open vertex shader file \"" << fileName << "\".\n";
		return "";
	}
	std::string vertexShaderText(
		(std::istreambuf_iterator<char>(vertexShaderFile)),
		(std::istreambuf_iterator<char>())
	);
	vertexShaderFile.close();
	return vertexShaderText;
}

void mainloop(ruya::MainWindow& mainWindow)
{
	// create the shader program
	ruya::Shader shader;
	try
	{
		shader.setShaders("source/shaders/vertex_shader.vert", "source/shaders/fragment_shader.frag");
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	shader.use();
	

	// create a vertex buffer object (VAO) so we don't have to repeat VBO and vertex attribute stuff
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// triangle data, location and color interleaved
	float vertices[] = {
		// vertex           // color
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left bottom
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // left top
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  // right top
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // right bottom
	};

	unsigned int indices[] = {  
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// element buffer
	unsigned int eboID;
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// vertex buffer
	unsigned int vboID;
	glGenBuffers(1, &vboID); // create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // set buffer's type to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW); // allocate memory and copy vertices to GPU

	// specify vertex attributes, how the data in the VBO should be evaluated
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // loc data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color data


	// activate the shader program and the vertex attribute settings
	shader.use();
	glBindVertexArray(vaoID);


	GLFWwindow* window = mainWindow.getGLFWWindowObj();
	glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f); // background color

	while (!mainWindow.shouldClose())
	{
		// change window color
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// RENDER THE RECTANGLE!!!
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// update frame
		glfwSwapBuffers(window); // swap buffer to the new to-be-rendered buffer

		// check for events
		glfwPollEvents(); // check if there are any events
		processInputs(window);
	}

}

void processInputs(GLFWwindow* window)
{
	// Close window if ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && allowRenderModeChange)
	{
		switch (renderMode)
		{
		case RenderMode::FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderMode = RenderMode::WIREFRAME;
			break;
		case RenderMode::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderMode = RenderMode::FILL;
			break;
		}

		allowRenderModeChange = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
	{
		allowRenderModeChange = true;
	}

}
