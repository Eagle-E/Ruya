#include "MainWindow.h"
#include <iostream>


MainWindow::MainWindow(int width, int height)
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

MainWindow::~MainWindow()
{
	glfwDestroyWindow(mGLFWwindow);
}

GLFWwindow* MainWindow::getGLFWWindowObj()
{
	return mGLFWwindow;
}

void MainWindow::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}