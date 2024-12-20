cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    Ruya
    VERSION 0.0.0
)



set(HEADERS
    engine/core/window.h
    engine/material.h
    engine/mesh.h
    engine/texture.h
    engine/scene/camera.h
    engine/scene/light_source.h
    engine/scene/object.h
    engine/scene/scene.h
    engine/scene/models/cube.h
    engine/scene/models/icosahedron.h
    engine/scene/models/square.h
    utils/uuid.h
    utils/timer.h
    io/stb_image.h
)
    
set(SOURCES
    main.cpp
    engine/core/window.cpp
    engine/material.cpp
    engine/mesh.cpp
    engine/texture.cpp
    engine/scene/camera.cpp
    engine/scene/light_source.cpp
    engine/scene/object.cpp
    engine/scene/scene.cpp
    engine/scene/models/cube.cpp
    engine/scene/models/icosahedron.cpp
    engine/scene/models/icosphere.hpp
    engine/scene/models/square.cpp
    utils/uuid.cpp
    utils/timer.cpp
    io/stb_image.cpp
)

# Compiler definitions
set(DEFINES

)

# Compiler options
set(OPTIONS

)

# targets to be used throughout our build process
set(MAIN_TARGET "main")
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# The source and header files defined here are assumed to be within the src/ folder
list(TRANSFORM HEADERS PREPEND "src/")
list(TRANSFORM SOURCES PREPEND "src/")

# create main target and add its sources
add_executable(${MAIN_TARGET})
target_sources(${MAIN_TARGET} PRIVATE ${SOURCES} ${HEADERS})

# add the src dir as include so that it can serve as the root for all our includes
#   =>  no matter where a source file is located within src/, it can include any other 
#       src file by providing the path starting from src/ as root, 
#           i.e. `#include "mydir/myfile.h"` includes the file located at src/mydir/myfile.h
target_include_directories(${MAIN_TARGET} PRIVATE ${SRC_DIR})



# dependencies
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")

# include the "external" dir
target_include_directories(${MAIN_TARGET} PRIVATE "${EXTERNAL_DIR}")
 


##### GLM
set(LINK_GLM_STATIC 0) # set to 1 if you want to link glm statically
set(GLM_DIR "${EXTERNAL_DIR}/glm")
if (LINK_GLM_STATIC)
    message(LINK_GLM_STATIC=${LINK_GLM_STATIC})
    set(BUILD_STATIC_LIBS 1)
    add_subdirectory(${GLM_DIR})
    target_link_libraries(${MAIN_TARGET} glm_static)
endif (LINK_GLM_STATIC)

##### GLFW3
set(GLFW_DIR "${EXTERNAL_DIR}/glfw")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${MAIN_TARGET} glfw)
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)



##### GLAD
set(GLAD_DIR "${EXTERNAL_DIR}/glad")

add_library(LibGlad "${GLAD_DIR}/src/glad.c")
target_include_directories(LibGlad PRIVATE "${GLAD_DIR}/include")

target_link_libraries(${MAIN_TARGET} LibGlad)
target_include_directories(${MAIN_TARGET} PRIVATE "${GLAD_DIR}/include")


##### IMGUI
#set(IMGUI_DIR "${EXTERNAL_DIR}/imgui")
#add_subdirectory("${IMGUI_DIR}")
#target_include_directories(${MAIN_TARGET} PRIVATE 
#    "${IMGUI_DIR}" 
#    "${IMGUI_DIR}/backends"
#    "${IMGUI_DIR}/misc/cpp"
#)
#target_link_libraries(${MAIN_TARGET} PRIVATE LibImGui)

if (WIN32)
endif (WIN32)


if (UNIX)

endif (UNIX)