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
#include <memory>

// my headers
#include "Window.h"
#include "Shader.h"
#include "Object.h"
#include "Square.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"

// namespace qualifiers
using std::vector;
using glm::vec3;
using ruya::Square;
using ruya::Shader;
using ruya::Mesh;
using ruya::Renderer;
using ruya::Texture;

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
	Shader shader;
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
	
	// init renderer
	Renderer renderer(shader);

	// the object to render
	Square square1;
	Square square2;
	shared_ptr<Texture> tex2 = std::make_shared<Texture>("resources/Leather026_1K-PNG/Leather026_1K_Color.png");
	shared_ptr<Texture> tex1 = std::make_shared<Texture>("resources/Wood049_1K-PNG/Wood049_1K_Color.png");
	square1.set_texture(tex1);
	square2.set_texture(tex2);

	// activate the shader program and the vertex attribute settings
	shader.use();
	
	// texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // wrap around in s- and t-axi
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // nearest neighbor filtering with best fitting mipmap when minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilinear interpolation when magnifying



	glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f); // background color
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
		renderer.render_object(square1);

		// second container
		glm::mat4 transform(1.0f);
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
		transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, 0.f));
		shader.setMatrix4D("transform", transform);
		shader.setMatrix4D("transform", transform);

		// RENDER THE RECTANGLE!!!
		renderer.render_object(square2);
		
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


