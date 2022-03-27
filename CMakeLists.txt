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


add_executable(OpenGL_tutorial_006_shaders 
tutorial_006_shaders.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
shader_006.h
Shaders/clouds.vs
Shaders/clouds.fs)
target_include_directories(OpenGL_tutorial_006_shaders PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_006_shaders ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_007_shaders_sea 
tutorial_007_shaders_sea.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
shader_006.h
Shaders/sea.vs
Shaders/sea.fs)
target_include_directories(OpenGL_tutorial_007_shaders_sea PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_007_shaders_sea ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_008_shaders_rainforest 
tutorial_008_shaders_rainforest.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/rainforest.vs
Shaders/rainforest.fs)
target_include_directories(OpenGL_tutorial_008_shaders_rainforest PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_008_shaders_rainforest ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


add_executable(OpenGL_tutorial_009_shaders_rippleCam 
tutorial_009_shaders_rippleCam.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/rippleCam.vs
Shaders/rippleCam.fs)
target_include_directories(OpenGL_tutorial_009_shaders_rippleCam PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_009_shaders_rippleCam ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()



add_executable(OpenGL_tutorial_010_shaders_multiple_3d_shapes 
tutorial_010_shaders_multiple_3d_shapes.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/multiple_3d_shapes.vs
Shaders/multiple_3d_shapes.fs)
target_include_directories(OpenGL_tutorial_010_shaders_multiple_3d_shapes PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_010_shaders_multiple_3d_shapes ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()

add_executable(OpenGL_tutorial_011_shaders_happy_jumping 
tutorial_011_shaders_happy_jumping.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/happy_jumping.vs
Shaders/happy_jumping.fs)
target_include_directories(OpenGL_tutorial_011_shaders_happy_jumping PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_011_shaders_happy_jumping ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()

add_executable(OpenGL_tutorial_012_shaders_using_mouse 
tutorial_012_shaders_using_mouse.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/using_mouse.vs
Shaders/using_mouse.fs)
target_include_directories(OpenGL_tutorial_012_shaders_using_mouse PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_012_shaders_using_mouse ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()

add_executable(OpenGL_tutorial_013_shaders_ray_tracing_two_spheres_and_walls 
tutorial_013_shaders_ray_tracing_two_spheres_and_walls.cpp
Third_party/Includes/glad/glad.h
Third_party/Includes/GLFW/glfw3.h
Third_party/Includes/GLFW/glfw3native.h
Third_party/Includes/KHR/khrplatform.h
Third_party/glad/src/glad.c
stb_image.cpp
stb_image.h
shader_006.h
Shaders/ray_tracing_two_spheres_and_walls.vs
Shaders/ray_tracing_two_spheres_and_walls.fs)
target_include_directories(OpenGL_tutorial_013_shaders_ray_tracing_two_spheres_and_walls PUBLIC Third_party/Includes .)
if(WIN32)
target_link_libraries(OpenGL_tutorial_013_shaders_ray_tracing_two_spheres_and_walls ${CMAKE_CURRENT_SOURCE_DIR}/Third_party/Libs/glfw3.lib)
endif()


