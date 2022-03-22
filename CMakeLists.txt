cmake_minimum_required(VERSION 3.0.0)

project(OpenGL_tutorials_solution VERSION 1.0.0)

add_executable(OpenGL_tutorial_001_triangle 
tutorial_001_triangle.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c)
target_include_directories(OpenGL_tutorial_001_triangle PUBLIC Third_party/Includes)
if(WIN32)
target_link_libraries(OpenGL_tutorial_001_triangle ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_002_rectangle 
tutorial_002_rectangle.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c)
target_include_directories(OpenGL_tutorial_002_rectangle PUBLIC Third_party/Includes)
if(WIN32)
target_link_libraries(OpenGL_tutorial_002_rectangle ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_003_shaders 
tutorial_003_shaders.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c)
target_include_directories(OpenGL_tutorial_003_shaders PUBLIC Third_party/Includes)
if(WIN32)
target_link_libraries(OpenGL_tutorial_003_shaders ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_004_shaders 
tutorial_004_shaders.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c)
target_include_directories(OpenGL_tutorial_004_shaders PUBLIC Third_party/Includes)
if(WIN32)
target_link_libraries(OpenGL_tutorial_004_shaders ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_005_shaders 
tutorial_005_shaders.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
learnopengl/shader_s.h
Shaders/3.3.shader.vs
Shaders/3.3.shader.fs)
target_include_directories(OpenGL_tutorial_005_shaders PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_005_shaders ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()

