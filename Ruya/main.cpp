#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "MainWindow.h"

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
	GLFWwindow* window = mainWindow.getGLFWWindowObj();
	glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f); // background color
	
	// triangle data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	glm::vec4 tcolor(0.0f, 0.0f, 0.0f, 1.0f);

	// read vertex shader file contents and setup vertex shader
	std::string vertexShaderText = readFileContents("vertex_shader.vert");
	unsigned int vertexShaderID;
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	const char* temp = vertexShaderText.c_str();
	glShaderSource(vertexShaderID, 1, &temp, NULL);
	glCompileShader(vertexShaderID);

	{ // errror checking for shader compilation
		int success;
		char textBuffer[512];
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, NULL, textBuffer);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << textBuffer << std::endl;
			return;
		}
	}

	// setup fragment shader
	std::string fragmentShaderText = readFileContents("fragment_shader.frag");
	unsigned int fragmentShaderID;
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	temp = fragmentShaderText.c_str();
	glShaderSource(fragmentShaderID, 1, &temp, NULL);
	glCompileShader(fragmentShaderID);

	{ // errror checking for shader compilation
		int success;
		char textBuffer[512];
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, textBuffer);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << textBuffer << std::endl;
			return;
		}
	}

	// link shaders into one shader program to be used for the render calls
	unsigned int shaderProgramID;
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	{ // errror checking for program linkage
		int success;
		char textBuffer[512];
		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, textBuffer);
			std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << textBuffer << std::endl;
			return;
		}
	}

	// delete shaders, not needed afer successful linkage
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	// create a vertex buffer object (VAO) so we don't have to repeat VBO and vertex attribute stuff
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// triangle buffer
	unsigned int vboID;
	glGenBuffers(1, &vboID); // create a buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // set buffer's type to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW); // allocate memory and copy vertices to GPU

	// specify vertex attributes, how the data in the VBO should be evaluated
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// activate the shader program and the vertex attribute settings
	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(0);


	while (!mainWindow.shouldClose())
	{
		// change window color
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// RENDER THE TRIANGLE!!!
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
}
