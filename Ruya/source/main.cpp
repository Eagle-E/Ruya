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
#include "Camera.h"
#include "Cube.h"

// namespace qualifiers
using std::vector;
using glm::vec3;
using ruya::Square;
using ruya::Shader;
using ruya::Mesh;
using ruya::Renderer;
using ruya::Texture;
using ruya::Camera;
using ruya::Cube;

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

//TODO: temporary, remove these camera move functions in the future
Camera camera;
float turnAnglePerSeconds = 1.0f;
float delta_seconds = 0.0f;

void go_up()
{
	camera.go_up();
}

void go_down()
{
	camera.go_down();
}

void go_forward()
{
	camera.go_forward();
}

void go_backward()
{
	camera.go_backward();
}

void go_left()
{
	camera.go_left();
}

void go_right()
{
	camera.go_right();
}

void turn_right()
{
	float angle = turnAnglePerSeconds * delta_seconds;
	camera.update_angle(angle, 0);
}

void turn_left()
{
	float angle = turnAnglePerSeconds * delta_seconds;
	camera.update_angle(-angle, 0);

}

void turn_up()
{
	float angle = turnAnglePerSeconds * delta_seconds;
	camera.update_angle(0, -angle);

}

void turn_down()
{
	float angle = turnAnglePerSeconds * delta_seconds;
	camera.update_angle(0, angle);

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
	//Camera camera;
	Renderer renderer(shader, window, camera);

	// set window event callbacks
	//typedef void (Camera::* func_ptr)();
	//func_ptr forward = &Camera::go_forward;
	//(camera.*forward)();

	window.add_event_callback(GLFW_KEY_W, &go_forward);
	window.add_event_callback(GLFW_KEY_S, &go_backward);
	window.add_event_callback(GLFW_KEY_A, &go_left);
	window.add_event_callback(GLFW_KEY_D, &go_right);
	window.add_event_callback(GLFW_KEY_SPACE, &go_up);
	window.add_event_callback(GLFW_KEY_LEFT_SHIFT, &go_down);
	window.add_event_callback(GLFW_KEY_UP, &turn_up);
	window.add_event_callback(GLFW_KEY_DOWN, &turn_down);
	window.add_event_callback(GLFW_KEY_LEFT, &turn_left);
	window.add_event_callback(GLFW_KEY_RIGHT, &turn_right);

	// the object to render
	Square square1;
	Square square2;
	shared_ptr<Texture> tex1 = std::make_shared<Texture>("resources/Wood049_1K-PNG/Wood049_1K_Color.png");
	shared_ptr<Texture> tex2 = std::make_shared<Texture>("resources/Leather026_1K-PNG/Leather026_1K_Color.png");
	square1.set_texture(tex1);
	square2.set_texture(tex2);

	Cube cube1;
	cube1.set_texture(tex1);

	// activate the shader program and the vertex attribute settings
	shader.use();
	
	glm::vec4 bgColor(1.0f, 1.0f, 1.0f, 1.0f); // background color
	double lastTime = glfwGetTime();
	double t0 = lastTime;
	int c = 0;
	while (!window.shouldClose())
	{
		// change window color
		glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// some transforming
		square1.set_rotation(glm::vec4(0.0f, 0.0f, 1.0f, glm::degrees((float)glfwGetTime())));
		square1.set_position(glm::vec3(2.5f, -2.5f, 0.0f));
		
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		square2.set_position(glm::vec3(-2.5f, 2.5f, 0.0f));
		square2.set_scale(glm::vec3(scaleAmount, scaleAmount, 0.f));

		// RENDER!!!
		renderer.render_object(square1);
		renderer.render_object(square2);
		renderer.render_object(cube1);

		// update frame => swaps buffers = starts showing newly rendered buffer
		// + checks for input events and calls handlers
		window.update();

		// calc FPS
		double newTime = glfwGetTime();
		double delta = newTime - lastTime;
		double fps = 1 / delta * 1000;
		//std::cout << fps << " fps\n";
		lastTime = newTime;
		delta_seconds = delta;

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


