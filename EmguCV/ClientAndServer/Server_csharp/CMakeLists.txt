cmake_minimum_required(VERSION 3.8.2)

project(EosimSdkServerSolution VERSION 1.0.0 LANGUAGES CSharp)

include(CSharpUtilities)

set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set(my_config "Debug")

file(GLOB IMPLEMENTATION_FILES "src/PluginEosimImp/*.cs")
ADD_LIBRARY(PluginEosimImpProject SHARED ${IMPLEMENTATION_FILES})
target_compile_options(PluginEosimImpProject PUBLIC "/unsafe")
if(WIN32)
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "./dlls/Emgu/Emgu.CV.World.dll")
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_System.Drawing "./dlls/System.Drawing.dll")
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_System.Xml "./dlls/System.Xml.dll") 
endif(WIN32)

file(GLOB SOURCES_FILES
	"src/PluginProtocolSwagger/AutoGen/Models/*.cs"
	"src/PluginProtocolSwagger/AutoGen/Modules/*.cs"
	"src/PluginProtocolSwagger/AutoGen/Utils/*.cs"
	"src/PluginProtocolSwagger/Adaptors/*.cs"
	"src/PluginProtocolSwagger/ServerUtils/*.cs"
	"src/PluginProtocolSwagger/Swagger/*.cs"
)	

foreach (SUBSOURCE ${SOURCES_FILES})
	message(STATUS "SUBSOURCE = ${SUBSOURCE}")
endforeach(SUBSOURCE)

message(STATUS "SOURCES_FILES = ${SOURCES_FILES}")



ADD_LIBRARY(PluginProtocolSwaggerProject SHARED ${SOURCES_FILES})

if(WIN32)
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCES "Microsoft.CSharp;System;System.XML")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "./dlls/Emgu/Emgu.CV.World.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Sharpility "./dlls/Sharpility.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_NodaTime "./dlls/NodaTime.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy "./dlls/Nancy.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy.Hosting.Self "./dlls/Nancy.Hosting.Self.dll")	
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Newtonsoft.Json "./dlls/Newtonsoft.Json.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_System.Collections.Immutable "./dlls/System.Collections.Immutable.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_System.Drawing "./dlls/System.Drawing.dll")
endif(WIN32)

target_link_libraries(PluginProtocolSwaggerProject PUBLIC PluginEosimImpProject)

#C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1\System.dll

file(GLOB PROTOCOL_PLUGIN_FILES "src/ProtocolPluginLoader/*.cs")
ADD_LIBRARY(ProtocolPluginLoaderProject SHARED ${PROTOCOL_PLUGIN_FILES})
	
target_link_libraries(ProtocolPluginLoaderProject PUBLIC PluginProtocolSwaggerProject)

add_executable(ServerApp src/ServerApp/Program.cs)
target_link_libraries(ServerApp PUBLIC ProtocolPluginLoaderProject)

function(copy_dll_files source_folder dest_folder)
	file(GLOB DLLS_FILES "${source_folder}/*.dll")
	foreach(DLL_SINGLE_FILE ${DLLS_FILES})
		if(WIN32)
			message(STATUS "DLL_SINGLE_FILE=${DLL_SINGLE_FILE} from ${source_folder} to ${dest_folder}")
			add_custom_command(TARGET ServerApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLL_SINGLE_FILE} ${dest_folder})
		endif(WIN32)
	endforeach(DLL_SINGLE_FILE)
endfunction()


copy_dll_files(${PROJECT_SOURCE_DIR}/dlls ${PROJECT_BINARY_DIR}/${my_config})
copy_dll_files(${PROJECT_SOURCE_DIR}/dlls/Emgu ${PROJECT_BINARY_DIR}/${my_config})


message(STATUS "CMAKE_COMMAND=${CMAKE_COMMAND}")
message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")


message(STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}/build/${my_config}/")

#[[
file(GLOB OUTPUT_FILES "${PROJECT_SOURCE_DIR}/build/${my_config}/*")
foreach(OUTPUT_SINGLE_FILE ${OUTPUT_FILES})
    if(WIN32)
        add_custom_command(TARGET ServerApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_SINGLE_FILE} ${PROJECT_SOURCE_DIR}/build/david)
    endif(WIN32)
endforeach(OUTPUT_SINGLE_FILE)
]]

#add_custom_command(TARGET ServerApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/build/${my_config} ${PROJECT_SOURCE_DIR}/build/david)




