cmake_minimum_required(VERSION 3.0.0)

project(EosimSdkServerSolution VERSION 1.0.0 LANGUAGES CSharp)

include(CSharpUtilities)

ADD_LIBRARY(PluginEosimImpProject SHARED PluginEosimImp/PolygonsLosModuleImplementation.cs)
if(WIN32)
	set_property(TARGET PluginEosimImpProject PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
endif(WIN32)

    file(GLOB SOURCES_FILES
		"PluginProtocolSwagger/AutoGen/Models/*.cs"
		"PluginProtocolSwagger/AutoGen/Modules/*.cs"
		"PluginProtocolSwagger/AutoGen/Utils/*.cs"
    )	
	
	foreach (SUBSOURCE ${SOURCES_FILES})
		message(STATUS "SUBSOURCE = ${SUBSOURCE}")
	endforeach(SUBSOURCE)
	
	message(STATUS "SOURCES_FILES = ${SOURCES_FILES}")

ADD_LIBRARY(PluginProtocolSwaggerProject SHARED ${SOURCES_FILES})

if(WIN32)
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Sharpility "../dlls/Sharpility.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_NodaTime "../dlls/NodaTime.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy "../dlls/Nancy.dll")
set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Nancy.Hosting.Self "../dlls/Nancy.Hosting.Self.dll")	
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_Newtonsoft.Json "../dlls/Newtonsoft.Json.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY VS_DOTNET_REFERENCE_System.Collections.Immutable "../dlls/System.Collections.Immutable.dll")
	set_property(TARGET PluginProtocolSwaggerProject PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
endif(WIN32)
	





