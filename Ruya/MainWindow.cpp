#include "MainWindow.h"
#include <iostream>


ruya::MainWindow::MainWindow(int width, int height)
	: mWidth(width), mHeight(height)
{
	mGLFWwindow = glfwCreateWindow(width, height, "opengl window", nullptr, nullptr);
	if (mGLFWwindow == nullptr)
	{
		// TODO: make it possible to check whether the creation of the window glfw object has failed or not.
		std::cerr << "Could not create GLFW window." << std::endl;
		return;
	}

	// opengl settings
	glViewport(0, 0, 1480, 720);

	// callback when window is resized
	glfwSetFramebufferSizeCallback(mGLFWwindow, windowResizeCallback); // when the window gets resized, the viewport has to be updated

}

ruya::MainWindow::~MainWindow()
{
	glfwDestroyWindow(mGLFWwindow);
}

GLFWwindow* ruya::MainWindow::getGLFWWindowObj()
{
	return mGLFWwindow;
}

void ruya::MainWindow::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}