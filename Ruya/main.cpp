#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "MainWindow.h"

void processInputs(GLFWwindow* window);

int main()
{
	// Hint windowing system the version of opengl used. This code is tested on gtx 1050m which supports
	// opengl 4.6 at the moment.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	MainWindow mainWindow;
	GLFWwindow* window = mainWindow.getGLFWWindowObj();
	glfwMakeContextCurrent(window);

	// init functions pointers to opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD (which is responsible for the opengl function pointers)" << std::endl;
		return -1;
	}

	// main loop
	glm::vec4 color(0.0f,0.0f,0.0f, 1.0f);
	int c = 0;
	float step = 0.01f;
	while (!mainWindow.shouldClose())
	{
		// change window color
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// update color vector for next frame
		if (c == 0)
		{
			color.r += step;
			color.g = 0;
			color.b = 0;

			if (color.r >= 0.99f)
			{
				color.r = 0;
				c = (c + 1) % 3;
			}
		}
		else if (c == 1)
		{
			color.r = 0;
			color.g += step;
			color.b = 0;

			if (color.g >= 0.99f)
			{
				color.g = 0;
				c = (c + 1) % 3;
			}
		}
		else if (c == 2)
		{
			color.r = 0;
			color.g = 0;
			color.b += step;

			if (color.b >= 0.99f)
			{
				color.b = 0;
				c = (c + 1) % 3;
			}
		}

		// update frame
		glfwSwapBuffers(window); // swap buffer to the new to-be-rendered buffer
		
		// check for events
		glfwPollEvents(); // check if there are any events
		processInputs(window);
	}

	glfwTerminate(); // clean up all reasources allocated by glfw.
	return 0;
}

void processInputs(GLFWwindow* window)
{
	// Close window if ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}