#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

AUTOGENERATED_PATH="$SCRIPT_DIR/AutoGen1"
SPEC_PATH="$SCRIPT_DIR/spec/los_polygons.spec"
NAMESPACE="EOSimU.API.AutoGen"
PKG_CTX="v1alpha3"

rm -rf $AUTOGENERATED_PATH/Modules
rm -rf $AUTOGENERATED_PATH/Models
rm -rf $AUTOGENERATED_PATH/Utils

mkdir -p $AUTOGENERATED_PATH
mkdir -p ${SCRIPT_DIR}/tmp_build

java -jar spec/openapi-generator-cli.jar generate \
	-i ${SPEC_PATH} \
	-g csharp-nancyfx \
	-o ${SCRIPT_DIR}/tmp_build \
	--additional-properties=asyncServer=false,packageName=$NAMESPACE,packageContext=$PKG_CTX

# Apply moves
mv ${SCRIPT_DIR}/tmp_build/* $AUTOGENERATED_PATH
rm -rf ${SCRIPT_DIR}/tmp_build

find "${AUTOGENERATED_PATH}/" -iname "*.cs" | xargs sed -i 's/using ;//'
