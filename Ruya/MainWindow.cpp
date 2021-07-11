#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height)
{
	mGLFWwindow = glfwCreateWindow(width, height, "opengl window", nullptr, nullptr);
	if (mGLFWwindow == nullptr)
	{
		// TODO: make it possible to check whether the creation of the window glfw object has failed or not.
		std::cerr << "Could not create GLFW window." << std::endl;
	}
}

GLFWwindow* MainWindow::getGLFWWindowObj()
{
	return mGLFWwindow;
}
