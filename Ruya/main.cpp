#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void windowResizeCallback(GLFWwindow* window, int width, int height);
void processInputs(GLFWwindow* window);

int main()
{
	// THESE VERSIONS ARE FOR THE INTEGRATED INTEL GPU, THE SECOND NVIDIA GPU MIGHT HAVE ANOTHER VERSION
	// CHECK THAT BEFORE PROCEDING WITH NVIDIA GPU IN THE FUTURE
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(1480, 720, "opengl window", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init functions pointers to opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD (which is responsible for the opengl function pointers)" << std::endl;
		return -1;
	}

	// opengl settings
	glViewport(0, 0, 1480, 720);
	glfwSetFramebufferSizeCallback(window, windowResizeCallback); // when the window gets resized, the viewport has to be updated

	// main loop
	glm::vec4 color(0.0f,0.0f,0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// change window color
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// update color vector for next frame
		if (color.b < 0.99f)
			color.b += 0.0004f;
		else
		{
			if (color.g < 0.99f)
				color.g += 0.0004f;
			else
			{
				if (color.r < 0.99f)
					color.r += 0.0004f;
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

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInputs(GLFWwindow* window)
{
	// Close window if ESC key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}