#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GLFW/glfw3.h>


/*
* The MainWindow object encapsulates GLFW code to create and manage a window.
*/
namespace ruya
{

	class Window
	{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Window(int width = 1480, int height = 720);
		~Window();

		// GETTERS & SETTERS
		GLFWwindow* getGLFWWindowObj();
		int height() { return mHeight; }
		int width() { return mWidth; }
		float aspect_ratio() { return static_cast<float>(mWidth) / static_cast<float>(mHeight); }

		// QUERRIES
		bool shouldClose() { return glfwWindowShouldClose(mGLFWwindow); }

		// MANIPULATORS
		void makeContextCurrent() { glfwMakeContextCurrent(mGLFWwindow); }
		void update();

	private:
		enum class RenderMode { FILL, WIREFRAME };
		
		// VARIABLES
		int mWidth = 0;
		int mHeight = 0;
		GLFWwindow* mGLFWwindow = nullptr;
		RenderMode renderMode = RenderMode::FILL;
		bool allowRenderModeChange = true;

		// PRIVATE FUNCTIONS
		static void windowResizeCallback(GLFWwindow* window, int width, int height);
		void processInputs(GLFWwindow* window);
	};


} // end namespace 


#endif // !MAINWINDOW_H