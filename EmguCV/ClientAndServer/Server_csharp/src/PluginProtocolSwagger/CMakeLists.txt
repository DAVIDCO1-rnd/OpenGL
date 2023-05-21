get_filename_component(target_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SOURCES_FILES
	"./AutoGen/Models/*.cs"
	"./AutoGen/Modules/*.cs"
	"./AutoGen/Utils/*.cs"
	"./Adaptors/*.cs"
	"./ServerUtils/*.cs"
	"./Swagger/*.cs"
)	

foreach (SUBSOURCE ${SOURCES_FILES})
	message(STATUS "SUBSOURCE = ${SUBSOURCE}")
endforeach(SUBSOURCE)

message(STATUS "SOURCES_FILES = ${SOURCES_FILES}")



ADD_LIBRARY(${target_name} SHARED ${SOURCES_FILES})

if(WIN32)
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCES "Microsoft.CSharp;System;System.XML")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_Emgu.CV.World "../../dlls/Emgu/Emgu.CV.World.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_Sharpility "../../dlls/Sharpility.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_NodaTime "../../dlls/NodaTime.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_Nancy "../../dlls/Nancy.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_Nancy.Hosting.Self "../../dlls/Nancy.Hosting.Self.dll")	
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_Newtonsoft.Json "../../dlls/Newtonsoft.Json.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_System.Collections.Immutable "../../dlls/System.Collections.Immutable.dll")
	set_property(TARGET ${target_name} PROPERTY VS_DOTNET_REFERENCE_System.Drawing "../../dlls/System.Drawing.dll")
endif(WIN32)

target_link_libraries(${target_name} PUBLIC "PluginEosimImp")
