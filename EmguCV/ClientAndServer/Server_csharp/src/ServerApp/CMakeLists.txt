get_filename_component(target_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB SRC_FILES "./*.cs")
add_executable(${target_name} ${SRC_FILES})
target_link_libraries(${target_name} PUBLIC "ProtocolPluginLoader")
add_custom_command(TARGET ${target_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dlls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)



