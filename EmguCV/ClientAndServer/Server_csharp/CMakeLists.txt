cmake_minimum_required(VERSION 3.8.2)

project(EosimSdkServerSolution VERSION 1.0.0 LANGUAGES CSharp)

include(CSharpUtilities)

set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")

file(GLOB IMAPLEMENTATION_FILES "src/PluginEosimImp/*.cs")
ADD_LIBRARY(PluginEosimImpProject SHARED ${IMAPLEMENTATION_FILES})
target_compile_options(PluginEosimImpProject PUBLIC "/unsafe")
if(WIN32)
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "C:/EmguCV/bin/Emgu.CV.World.dll")
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_cvextern "C:/EmguCV/bin/x64/cvextern.dll")	
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
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCES "Microsoft.CSharp;System")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "C:/EmguCV/bin/Emgu.CV.World.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Sharpility "./dlls/Sharpility.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_NodaTime "./dlls/NodaTime.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy "./dlls/Nancy.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy.Hosting.Self "./dlls/Nancy.Hosting.Self.dll")	
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Newtonsoft.Json "./dlls/Newtonsoft.Json.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_System.Collections.Immutable "./dlls/System.Collections.Immutable.dll")
endif(WIN32)

target_link_libraries(PluginProtocolSwaggerProject PUBLIC PluginEosimImpProject)

#C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1\System.dll

file(GLOB PROTOCOL_PLUGIN_FILES "src/ProtocolPluginLoader/*.cs")
ADD_LIBRARY(ProtocolPluginLoaderProject SHARED ${PROTOCOL_PLUGIN_FILES})
	
target_link_libraries(ProtocolPluginLoaderProject PUBLIC PluginProtocolSwaggerProject)

add_executable(ServerApp src/ServerApp/Program.cs)
target_link_libraries(ServerApp PUBLIC ProtocolPluginLoaderProject)




