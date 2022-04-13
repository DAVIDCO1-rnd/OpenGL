#!/bin/bash

g++ -g -I Third_party/include -c Third_party/include/IMGUI/backends/imgui_impl_glfw.cpp Third_party/include/IMGUI/backends/imgui_impl_opengl3.cpp Third_party/include/glad/glad.c

opencvLibs="libopencv_calib3d, libopencv_core, libopencv_dnn, libopencv_features2d, libopencv_flann, libopencv_gapi, libopencv_highgui, libopencv_imgcodecs, libopencv_imgproc, libopencv_ml, libopencv_objdetect, libopencv_photo, libopencv_stitching, libopencv_video, libopencv_videoio"

g++ -g -I Third_party/include -I include -I /usr/local/include/opencv4 src/main.cpp imgui_impl_opengl3.o imgui_impl_glfw.o glad.o /usr/local/lib/libopencv_calib3d.so, /usr/local/lib/libopencv_core.so, /usr/local/lib/libopencv_dnn.so, /usr/local/lib/libopencv_features2d.so, /usr/local/lib/libopencv_flann.so, /usr/local/lib/libopencv_gapi.so, /usr/local/lib/libopencv_highgui.so, /usr/local/lib/libopencv_imgcodecs.so, /usr/local/lib/libopencv_imgproc.so, /usr/local/lib/libopencv_ml.so, /usr/local/lib/libopencv_objdetect.so, /usr/local/lib/libopencv_photo.so, /usr/local/lib/libopencv_stitching.so, /usr/local/lib/libopencv_video.so, /usr/local/lib/libopencv_videoio.so -o main -lglfw -ldl -lGL
