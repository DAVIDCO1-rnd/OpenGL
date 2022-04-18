cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV included at ${OpenCV_INCLUDE_DIRS}")
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
if(APPLE)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype irrKlang)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -O0")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS )
endif(WIN32)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries requried for other projects
# add_library(STB_IMAGE "src/stb_image.cpp")
# set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "Third_party/include/IMGUI/backends/imgui_impl_glfw.cpp" "Third_party/include/IMGUI/backends/imgui_impl_opengl3.cpp")
set(LIBS ${LIBS} GLAD)

add_library(IMGUI "./Third_party/include/glad/glad.c")
set(LIBS ${LIBS} IMGUI)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

function(create_project_from_sources)
	file(GLOB SOURCE
            "include/*.h"
            "src/*.cpp"
            "src/shaders/*.vs"
            "src/shaders/*.fs"
            "src/shaders/*.gs"
    )
    foreach(SUBSOURCE ${SOURCE})
      message(STATUS "SUBSOURCE = ${SUBSOURCE}")
    endforeach(SUBSOURCE)
    set(NAME "my_project")
    add_executable(${NAME} ${SOURCE})

    #OpenCV_INCLUDE_DIRS is defined in /usr/local/lib/cmake/opencv4/OpenCVConfig.cmake line 25
    target_include_directories(${NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

    #OpenCV_LIBS is defined in /usr/local/lib/cmake/opencv4/OpenCVConfig.cmake line 24
    #target_link_libraries(${NAME} PRIVATE ${OpenCV_LIBS})

    target_link_libraries(${NAME} ${LIBS} ${OpenCV_LIBS})

    message("OpenCV Include Directories: ")
    foreach(OpenCV_INCLUDE_DIR ${OpenCV_INCLUDE_DIRS})
    message(${OpenCV_INCLUDE_DIR} ", ")
    endforeach(OpenCV_INCLUDE_DIR)

    message("OpenCV Libraries: ")
    foreach(OpenCV_LIB ${OpenCV_LIBS})
    set(ONE_LINER_OPENCVE_LIBS ${ONE_LINER_OPENCVE_LIBS} "/usr/local/lib/lib" ${OpenCV_LIB} ".so, ")
    endforeach(OpenCV_LIB)    
    message(${ONE_LINER_OPENCVE_LIBS})
    

    if(MSVC)
		target_compile_options(${NAME} PRIVATE /std:c++17 /MP INTERFACE -O0)
        target_link_options(${NAME} PUBLIC /ignore:4099)
    endif(MSVC)
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")
    endif(WIN32)
    # copy shader files to build directory
    file(GLOB SHADERS
             "src/shaders/*.vs"
             "src/shaders/*.fs"
             "src/shaders/*.gs"
    )
    foreach(SHADER ${SHADERS})
        if(WIN32)
            # configure_file(${SHADER} "test")
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${SHADERNAME} ${NAME})
        endif(WIN32)
    endforeach(SHADER)
    # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
endfunction()


create_project_from_sources()


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/Third_party/include)
