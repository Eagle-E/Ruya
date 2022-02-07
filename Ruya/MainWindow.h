#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GLFW/glfw3.h>


/*
* The MainWindow object encapsulates GLFW code to create and manage a window.
*/
namespace ruya
{

	class MainWindow
	{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		MainWindow(int width = 1480, int height = 720);
		~MainWindow();

		// GETTERS & SETTERS
		GLFWwindow* getGLFWWindowObj();
		int height() { return mHeight; }
		int width() { return mWidth; }
	
		// QUERRIES
		bool shouldClose() { return glfwWindowShouldClose(mGLFWwindow); }

	private:
		// VARIABLES
		int mWidth = 0;
		int mHeight = 0;
		GLFWwindow* mGLFWwindow = nullptr;

		// PRIVATE FUNCTIONS
		static void windowResizeCallback(GLFWwindow* window, int width, int height);
	};


} // end namespace 


#include <iostream>
static void test()
{
	std::cout << "The static function has been called\n";
}



#endif // !MAINWINDOW_H