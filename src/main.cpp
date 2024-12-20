#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "engine/core/window.h"
#include "engine/mesh.h"
#include "engine/texture.h"
#include "utils/uuid.h"
#include "utils/timer.h"

#include "engine/scene/camera.h"
#include "engine/scene/light_source.h"
#include "engine/scene/object.h"
#include "engine/scene/scene.h"
#include "engine/scene/models/cube.h"
#include "engine/scene/models/icosahedron.h"
#include "engine/scene/models/icosphere.hpp"
#include "engine/scene/models/square.h"

using namespace std;


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main()
{
    ruya::Mesh mesh1{};
    ruya::UUID id1{};
    ruya::Texture tex1{};
    ruya::Timer timer{};

    ruya::Window window{1000, 1000};
    ruya::Camera cam{};

    ruya::Object obj{};
    ruya::LightSource light{};
    ruya::Scene scene{};

    ruya::models::Cube cube{};
    ruya::models::Icosahedron icosahedron{};
    ruya::models::Icosphere icosphere{};
    ruya::models::Square square{};

    timer.start();
    timer.stop();


    while(!window.should_close())
    {
        glfwPollEvents();

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window.get_GLFW_window(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window.get_GLFW_window());
    }

    return 0;
}