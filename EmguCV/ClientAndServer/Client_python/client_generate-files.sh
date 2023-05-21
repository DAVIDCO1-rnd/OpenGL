#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
SPEC_PATH="$SCRIPT_DIR/../spec/los_polygons.spec"
CLIENT_GENERATED_CODE="${SCRIPT_DIR}/python_project/python_client_generated_code"

rm -rf "${CLIENT_GENERATED_CODE}"

java -jar ../spec/openapi-generator-cli.jar generate \
	-i "${SPEC_PATH}" \
	-g python \
	-o "${CLIENT_GENERATED_CODE}"
	
#cp -R src/* ${CLIENT_GENERATED_CODE}




