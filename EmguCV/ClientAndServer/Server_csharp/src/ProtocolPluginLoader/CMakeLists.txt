get_filename_component(target_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB PROTOCOL_PLUGIN_FILES "./*.cs")
ADD_LIBRARY(${target_name} SHARED ${PROTOCOL_PLUGIN_FILES})
	
target_link_libraries(${target_name} PUBLIC "PluginProtocolSwagger")

if(WIN32)
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCES "Microsoft.CSharp;System;System.XML")
endif(WIN32)


