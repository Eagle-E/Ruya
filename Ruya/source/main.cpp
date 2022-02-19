// library headers
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>

// my headers
#include "Window.h"
#include "Shader.h"
#include "Object.h"
#include "Square.h"

// namespace qualifiers
using std::vector;
using ruya::Square;
using ruya::Shader;
using glm::vec3;

// CODE
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
	/*float vertices[] = {
		// vertex           // color          // texture
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // left bottom
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // left top
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // right top
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f  // right bottom
	};*/


	float textureCoords[] = {
		0.0f, 0.0f, // left bottom
		0.0f, 1.0f, // left top
		1.0f, 1.0f, // right top
		1.0f, 0.0f  // right bottom
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
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW); // allocate memory and copy vertices to GPU
	// UPDATE: change interleaved buffered data into concatenated: 
	// v: vertex, c: color, t: texture   from vctvctvct to vvvcccttt
	Square square; 
	vector<vec3>& squareMesh = *square.get_mesh();
	int meshSize = squareMesh.size() * sizeof(squareMesh[0]);
	GLsizeiptr totalBytes = meshSize  + sizeof(textureCoords);

	glBufferData(GL_ARRAY_BUFFER, totalBytes, NULL, GL_STATIC_DRAW); // allocate memory and copy vertices to GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, meshSize, squareMesh.data());
	glBufferSubData(GL_ARRAY_BUFFER, meshSize , sizeof(textureCoords), textureCoords);

	// specify vertex attributes, how the data in the VBO should be evaluated
	// UPDATED according to the concatenated data format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // loc data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(meshSize)); // texture data
	glEnableVertexAttribArray(1);


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
	int width2, height2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load("resources/Wood049_1K-PNG/Wood049_1K_Color.png", &width, &height, &nrChannels, 0);
	unsigned char* texData2 = stbi_load("resources/awesomeface.png", &width2, &height2, &nrChannels2, 0);

	// create texture
	GLuint texID, texID2;
	if (texData /*&& texData2*/)
	{
		glGenTextures(1, &texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		GLuint sourceColorType;
		nrChannels == 3 ? sourceColorType = GL_RGB : sourceColorType = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, sourceColorType, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
		shader.setInt("ourTexture", 0);

		glGenTextures(1, &texID2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texID2);
		nrChannels2 == 3 ? sourceColorType = GL_RGB : sourceColorType = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, sourceColorType, GL_UNSIGNED_BYTE, texData2);
		glGenerateMipmap(GL_TEXTURE_2D);
		shader.setInt("ourTexture2", 1);
	}
	else
	{
		std::cerr << "Error loading texture: " << stbi_failure_reason() << std::endl;
	}

	// free image, not needed anymore
	stbi_image_free(texData);
	stbi_image_free(texData2);

	double lastTime = glfwGetTime();
	double t0 = lastTime;
	int c = 0;
	while (!window.shouldClose())
	{
		// change window color
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	
		// let's add a transformation matrix
		glm::mat4 transformation(1.0f);
		transformation = glm::rotate(transformation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformation = glm::translate(transformation, glm::vec3(0.5f, -0.5f, 0.0f)); // switched the order  
		shader.setMatrix4D("transform", transformation);

		// RENDER THE RECTANGLE!!!
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// second container
		glm::mat4 transform(1.0f);
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, 0.f));
		shader.setMatrix4D("transform", transform);
		shader.setMatrix4D("transform", transform);

		// RENDER THE RECTANGLE!!!
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// update frame => swaps buffers = starts showing newly rendered buffer
		// + checks for input events and calls handlers
		window.update();

		// calc FPS
		double newTime = glfwGetTime();
		double delta = newTime - lastTime;
		double fps = 1 / delta * 1000;
		//std::cout << fps << " fps\n";
		lastTime = newTime;

		double delta2 = newTime - t0;
		if (delta2 < 1)
			c++;
		else
		{
			std::cout << c << " fps\n";
			t0 = newTime;
			c = 0;
		}
	}

}


