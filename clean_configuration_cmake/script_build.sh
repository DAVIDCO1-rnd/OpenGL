#!/bin/bash

g++ -std=c++17 -Wall -g -I Third_party/include -c Third_party/include/IMGUI/backends/imgui_impl_glfw.cpp Third_party/include/IMGUI/backends/imgui_impl_opengl3.cpp Third_party/include/glad/glad.c

#g++ -g -I Third_party/include -I include -I /usr/local/include/opencv4 src/main.cpp imgui_impl_opengl3.o imgui_impl_glfw.o glad.o -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio -lglfw -ldl -lGL  -o main

g++ -std=c++17 -Wall -g -I Third_party/include -I include -I /usr/local/include/opencv4 src/main.cpp imgui_impl_opengl3.o imgui_impl_glfw.o glad.o -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio -lglfw -ldl -o main