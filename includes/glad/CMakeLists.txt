add_library(t_glad STATIC glad.c)
target_include_directories(t_glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")