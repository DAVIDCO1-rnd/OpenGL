#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"
#include "ModelParameters.h"

using json = nlohmann::json;

class ModelParameters {
public:
	GLfloat angleX;
	GLfloat angleY;
	GLfloat angleZ;
	GLfloat scaleX;
	GLfloat scaleY;
	GLfloat scaleZ;
	GLfloat scaleUniform; //for uniform scaling (in all the axises)
	GLfloat translateX;
	GLfloat translateY;
	GLfloat translateZ;


	ModelParameters() {
		angleX = 0.0f;
		angleY = 0.0f;
		angleZ = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
		scaleZ = 1.0f;
		scaleUniform = 1.0f; //for uniform scaling (in all the axises)
		translateX = 0.0f;
		translateY = 0.0f;
		translateZ = 0.0f;
	}
};


class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* file, std::string modelName);

	void Draw(Shader& shader, Camera& camera);
	ModelParameters getParams();
	std::string getModelName();	
	void setParams(ModelParameters params);
	void updateModelMatrixByUserParameters();
	glm::mat4 getModelMatrix();

private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	json JSON;
	ModelParameters params;
	std::string modelName;
	glm::mat4 modelMatrix;

	// All the meshes and transformations
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// Loads a single mesh by its index
	void loadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> getData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions, 
		std::vector<glm::vec3> normals, 
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif