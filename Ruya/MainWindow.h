#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GLFW/glfw3.h>



class MainWindow
{
public:
	MainWindow(int width = 1480, int height = 720);
	GLFWwindow* getGLFWWindowObj();

private:
	int mWidth = 1480;
	int mHeight = 720;
	GLFWwindow* mGLFWwindow = nullptr;
};



#endif // !MAINWINDOW_H