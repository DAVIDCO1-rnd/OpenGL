cmake_minimum_required(VERSION 3.0.0)

project(OpenGL_tutorials_solution VERSION 1.0.0)

add_executable(OpenGL_tutorial_001 
tutorial_001.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c)

target_include_directories(OpenGL_tutorial_001 PUBLIC Third_party/Includes)
if(WIN32)
target_link_libraries(OpenGL_tutorial_001 ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()




