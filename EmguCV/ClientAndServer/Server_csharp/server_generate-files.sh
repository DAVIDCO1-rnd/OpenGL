#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

DEST_CODE_PATH="$SCRIPT_DIR/src/PluginProtocolSwagger/AutoGen"
AUTOGENERATED_PATH="$SCRIPT_DIR/AutoGen"
SPEC_PATH="$SCRIPT_DIR/../spec/los_polygons.spec"
NAMESPACE="EOSimU.API.AutoGen"
PKG_CTX="v1alpha3"

rm -rf "$AUTOGENERATED_PATH"
mkdir -p "$AUTOGENERATED_PATH"

java -jar ../spec/openapi-generator-cli.jar generate \
	-i "${SPEC_PATH}" \
	-g csharp-nancyfx \
	-o "$AUTOGENERATED_PATH" \
	--additional-properties=asyncServer=false,packageName=$NAMESPACE,packageContext=$PKG_CTX

find "${AUTOGENERATED_PATH}/" -iname "*.cs" -exec printf "\"%s\"\n" {} \; | xargs sed -i 's/using ;//'


rm -rf "$DEST_CODE_PATH/Modules"
rm -rf "$DEST_CODE_PATH/Models"
rm -rf "$DEST_CODE_PATH/Utils"

mkdir -p "$DEST_CODE_PATH"

cp -R "$AUTOGENERATED_PATH/src/$NAMESPACE/Modules" "$DEST_CODE_PATH/Modules"
cp -R "$AUTOGENERATED_PATH/src/$NAMESPACE/Models" "$DEST_CODE_PATH/Models"
cp -R "$AUTOGENERATED_PATH/src/$NAMESPACE/Utils" "$DEST_CODE_PATH/Utils"

rm -rf "$AUTOGENERATED_PATH"


cmake 	-B build \
		-G "Visual Studio 15 2017" \
		-A x64 
	
#cmake -G{GENERATOR_NAME} -B{BUILD_DIR_PATH} -H{SOURCE_DIR_PATH}	

#cmake -GVisual Studio 14 -Bbuild/win32/x86 -H.  

#MY_OUTPUT_PATH="D:\\BLABLA"
#echo $MY_OUTPUT_PATH
#sed -i "s/<OutputPath>.*<\/OutputPath>/<OutputPath>`echo ${MY_OUTPUT_PATH}`<\/OutputPath>/" build/ServerApp.csproj
#sed -i "s/<StartProgram>.*<\/StartProgram>/<StartProgram>`echo ${MY_OUTPUT_PATH}`\/ServerApp.exe<\/StartProgram>/" build/ServerApp.csproj



