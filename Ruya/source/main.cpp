#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shader.h"
#include "stb_image.h"

void mainloop(ruya::Window& window);

int main()
{
	// Hint windowing system the version of opengl used. This code is tested on gtx 1050m which supports
	// opengl 4.6 at the moment.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	ruya::Window window(720, 720);
	window.makeContextCurrent();

	// init functions pointers to opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD (which is responsible for the opengl function pointers)" << std::endl;
		return -1;
	}

	// main loop
	mainloop(window);

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

void mainloop(ruya::Window& window)
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

	// triangle data, location, color and texture coords interleaved
	float vertices[] = {
		// vertex           // color          // texture
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // left bottom
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // left top
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // right top
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f  // right bottom
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // loc data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture data


	// activate the shader program and the vertex attribute settings
	shader.use();
	glBindVertexArray(vaoID);
	
	glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f); // background color

	// texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // wrap around in s- and t-axi
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // nearest neighbor filtering with best fitting mipmap when minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilinear interpolation when magnifying

	// load texture data
	int width, height, nrChannels;
	unsigned char* texData = stbi_load("resources/Wood049_1K-PNG/Wood049_1K_Color.png", &width, &height, &nrChannels, 0);
	

	// create texture
	GLuint texID;
	if (texData)
	{
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Error loading texture: " << stbi_failure_reason() << std::endl;
	}

	// free image, not needed anymore
	stbi_image_free(texData);

	while (!window.shouldClose())
	{
		// change window color
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// RENDER THE RECTANGLE!!!
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// update frame => swaps buffers = starts showing newly rendered buffer
		// + checks for input events and calls handlers
		window.update();
	}

}


