cmake_minimum_required(VERSION 3.8.2)

project(EosimSdkServerSolution VERSION 1.0.0 LANGUAGES CSharp)

include(CSharpUtilities)

ADD_LIBRARY(PluginEosimImpProject SHARED PluginEosimImp/PolygonsLosModuleImplementation.cs)
if(WIN32)
	set_property(TARGET PluginEosimImpProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "C:/Emgu/libemgucv-windesktop-3.1.0.2504-r16.12/bin/Emgu.CV.World.dll")
	set_property(TARGET PluginEosimImpProject PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
endif(WIN32)

file(GLOB SOURCES_FILES
	"PluginProtocolSwagger/AutoGen/Adaptors/*.cs"
	"PluginProtocolSwagger/AutoGen/Models/*.cs"
	"PluginProtocolSwagger/AutoGen/Modules/*.cs"
	"PluginProtocolSwagger/AutoGen/Swagger/*.cs"
	"PluginProtocolSwagger/AutoGen/Utils/*.cs"
)	

foreach (SUBSOURCE ${SOURCES_FILES})
	message(STATUS "SUBSOURCE = ${SUBSOURCE}")
endforeach(SUBSOURCE)

message(STATUS "SOURCES_FILES = ${SOURCES_FILES}")

set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")

ADD_LIBRARY(PluginProtocolSwaggerProject SHARED ${SOURCES_FILES})

if(WIN32)
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCES "Microsoft.CSharp;System")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "C:/Emgu/libemgucv-windesktop-3.1.0.2504-r16.12/bin/Emgu.CV.World.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Sharpility "../dlls/Sharpility.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_NodaTime "../dlls/NodaTime.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy "../dlls/Nancy.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy.Hosting.Self "../dlls/Nancy.Hosting.Self.dll")	
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Newtonsoft.Json "../dlls/Newtonsoft.Json.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_System.Collections.Immutable "../dlls/System.Collections.Immutable.dll")
endif(WIN32)

target_link_libraries(PluginProtocolSwaggerProject PUBLIC PluginEosimImpProject)

#C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.6.1\System.dll

file(GLOB PROTOCOL_PLUGIN_FILES "ProtocolPluginLoader/*.cs")
ADD_LIBRARY(ProtocolPluginLoaderProject SHARED ${PROTOCOL_PLUGIN_FILES})
	
target_link_libraries(ProtocolPluginLoaderProject PUBLIC PluginProtocolSwaggerProject)

add_executable(ServerApp ServerApp/Program.cs)
target_link_libraries(ServerApp PUBLIC ProtocolPluginLoaderProject)




