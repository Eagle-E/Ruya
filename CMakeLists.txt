cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(
    Ruya
    VERSION 0.0.0
)



set(HEADERS
    mesh.h
    utils/uuid.h
)

set(SOURCES
    main.cpp
    mesh.cpp
    utils/uuid.cpp
)

# Compiler definitions
set(DEFINES

)

# Compiler options
set(OPTIONS

)

# targets to be used throughout our build process
set(MAIN_TARGET "main")

# The source and header files defined here are assumed to be within the src/ folder
list(TRANSFORM HEADERS PREPEND "src/")
list(TRANSFORM SOURCES PREPEND "src/")

# create main target and add its sources
add_executable(${MAIN_TARGET})
target_sources(${MAIN_TARGET} PRIVATE ${SOURCES} ${HEADERS})



# dependencies
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")

# include the "external" dir
target_include_directories(${MAIN_TARGET} PRIVATE "${EXTERNAL_DIR}")
 


##### GLM
set(GLM_DIR "${EXTERNAL_DIR}/glm")
target_include_directories(${MAIN_TARGET} PRIVATE "${GLM_DIR}")

##### GLFW3
set(GLFW_DIR "${EXTERNAL_DIR}/glfw")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${MAIN_TARGET} glfw)
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


##### GLAD
set(GLAD_DIR "${EXTERNAL_DIR}/glad")
add_subdirectory("${GLAD_DIR}")
target_include_directories(${MAIN_TARGET} PRIVATE "${GLAD_DIR}")
target_link_libraries(${MAIN_TARGET} LibGlad)


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