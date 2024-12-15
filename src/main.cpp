#include <iostream>
#include <glad.h> 
#include <GLFW/glfw3.h>

#include "engine/mesh.h"
#include "utils/uuid.h"

using namespace std;


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main()
{
    ruya::Mesh mesh1{};
    ruya::UUID id1{};



    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        cout << "[GLFW3] initialization failed, terminating program." << endl;
        return 1;
    }
    else
        cout << "[GLFW3] successfully initialized" << endl;

    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        cout << "[GLFW3] failed to create window, terminating program." << endl;
        return 1;
    }
    else
        cout << "[GLFW3] successfully created window." << endl;
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    else
        cout << "[GLAD] Successfully loaded opengl functions." << endl;

    // glViewport(0, 0, 800, 600);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}