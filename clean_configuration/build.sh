#!/bin/bash

g++ -g -I lib/include -I lib/include/GLAD -c lib/include/IMGUI/backends/imgui_impl_glfw.cpp lib/include/IMGUI/backends/imgui_impl_opengl3.cpp lib/include/GLAD/glad/glad.c
g++ -g -I lib/include -I lib/include/GLAD main.cpp imgui_impl_opengl3.o imgui_impl_glfw.o glad.o -o main -lglfw -ldl -lGL
