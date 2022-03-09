#include "Window.h"
#include <iostream>

ruya::Window::Window(int width, int height)
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
	glfwSetWindowUserPointer(mGLFWwindow, this);
	glfwSetFramebufferSizeCallback(mGLFWwindow, windowResizeCallbackStatic); // when the window gets resized, the viewport has to be updated
}

ruya::Window::~Window()
{
	glfwDestroyWindow(mGLFWwindow);
}

GLFWwindow* ruya::Window::getGLFWWindowObj()
{
	return mGLFWwindow;
}

void ruya::Window::update()
{
	// update frame
	glfwSwapBuffers(mGLFWwindow); // swap buffer to the new to-be-rendered buffer

	// check for events
	glfwPollEvents(); // check if there are any events
	processInputs();
}

void ruya::Window::add_event_callback(int glfwEventID, VOID_FPTR callback)
{
	// event type is not registered yet
	if (mEventCallbackMap.find(glfwEventID) == mEventCallbackMap.end())
	{
		mEventCallbackMap[glfwEventID] = list<VOID_FPTR>();
	}

	mEventCallbackMap[glfwEventID].push_back(callback);
}

void ruya::Window::remove_event_callback(VOID_FPTR callback)
{
	unordered_map<int, list<VOID_FPTR>>::iterator it = mEventCallbackMap.begin();
	for (it; it != mEventCallbackMap.end(); it++)
	{
		it->second.remove(callback);
	}
}

void ruya::Window::windowResizeCallbackStatic(GLFWwindow* window, int width, int height)
{
	static_cast<Window*>(glfwGetWindowUserPointer(window))->windowResizeCallback(window, width, height);
}

void ruya::Window::windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	mWidth = width;
	mHeight = height;
}


void ruya::Window::processInputs()
{		
	// Close window if ESC key is pressed
	if (glfwGetKey(mGLFWwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mGLFWwindow, true);

	if (glfwGetKey(mGLFWwindow, GLFW_KEY_1) == GLFW_PRESS && allowRenderModeChange)
	{
		switch (renderMode)
		{
		case RenderMode::FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderMode = RenderMode::WIREFRAME;
			break;
		case RenderMode::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			renderMode = RenderMode::FILL;
			break;
		}

		allowRenderModeChange = false;
	}
	else if (glfwGetKey(mGLFWwindow, GLFW_KEY_1) == GLFW_RELEASE)
	{
		allowRenderModeChange = true;
	}

	call_handlers_on_key_press(GLFW_KEY_UP);
	call_handlers_on_key_press(GLFW_KEY_DOWN);
	call_handlers_on_key_press(GLFW_KEY_LEFT);
	call_handlers_on_key_press(GLFW_KEY_RIGHT);
	call_handlers_on_key_press(GLFW_KEY_W);
	call_handlers_on_key_press(GLFW_KEY_S);
	call_handlers_on_key_press(GLFW_KEY_A);
	call_handlers_on_key_press(GLFW_KEY_D);
	call_handlers_on_key_press(GLFW_KEY_SPACE);
	call_handlers_on_key_press(GLFW_KEY_LEFT_SHIFT);
}


void ruya::Window::call_handlers_on_key_press(int glfwKeyEventID)
{
	if (glfwGetKey(mGLFWwindow, glfwKeyEventID) == GLFW_PRESS)
	{
		for (auto callback : mEventCallbackMap[glfwKeyEventID])
			callback();
	}
}