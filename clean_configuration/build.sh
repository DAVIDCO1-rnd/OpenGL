#!/bin/bash

g++ -g -I lib/include -c lib/include/IMGUI/backends/imgui_impl_glfw.cpp lib/include/IMGUI/backends/imgui_impl_opengl3.cpp
g++ -g -I lib/include main.cpp imgui_impl_opengl3.o imgui_impl_glfw.o -o main -lglfw -ldl -lGL
