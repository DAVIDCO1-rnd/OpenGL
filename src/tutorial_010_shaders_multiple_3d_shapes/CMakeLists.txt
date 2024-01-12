get_filename_component(target_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(main_include_folder "${CMAKE_SOURCE_DIR}/include")
set(include_folder "${CMAKE_SOURCE_DIR}/include/${target_name}")
set(stb_image_folder "${CMAKE_SOURCE_DIR}/stb_image")
set(src_folder "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB HEADERS_FILES "${include_folder}/*.h")
file(GLOB SOURCES_FILES "${src_folder}/*.cpp")

add_executable(${target_name} 
    ${SOURCES_FILES} 
    ${HEADERS_FILES}
    ${CMAKE_SOURCE_DIR}/Third_party/Includes/glad/glad.h
    ${CMAKE_SOURCE_DIR}/Third_party/Includes/GLFW/glfw3.h
    ${CMAKE_SOURCE_DIR}/Third_party/Includes/GLFW/glfw3native.h
    ${CMAKE_SOURCE_DIR}/Third_party/Includes/KHR/khrplatform.h
    ${CMAKE_SOURCE_DIR}/Third_party/glad/src/glad.c
    ${stb_image_folder}/stb_image.cpp
    ${stb_image_folder}/stb_image.h
    ${CMAKE_SOURCE_DIR}/resources/shaders/multiple_3d_shapes.vs
    ${CMAKE_SOURCE_DIR}/resources/shaders/multiple_3d_shapes.fs
)

target_include_directories(${target_name} PUBLIC 
    ${main_include_folder} 
    ${stb_image_folder}
    ${CMAKE_SOURCE_DIR}/Third_party/Includes
)

target_link_libraries(${target_name}
    ${CMAKE_SOURCE_DIR}/Third_party/Libs/glfw3.lib
)

target_compile_features(${target_name} PUBLIC 
cxx_std_17
)
