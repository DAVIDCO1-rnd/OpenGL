#pragma once
#include "Object.h"
#include <vector>
#include "ModelParameters.h"
#include "OpenGLErrors.h"
#include <iostream>

using std::vector;


class Mesh
{
protected:
	string modelName;
	string fileName;
	vector<unsigned int> indices;
	vector<float> vertices;

	cv::Mat1i indicesOpenCV;
	cv::Mat1d verticesOpenCV; 
	

	glm::mat4 modelMatrix;
	ModelParameters params;

	bool loadOBJ(string path, vector<float>& out_vertices, vector<unsigned int>& out_faces)
	{
		//printf("Loading OBJ file %s...\n", path.c_str());

		FILE * file = fopen(path.c_str(), "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}

		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) {
				vector<float> vertex(3);
				fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				out_vertices.push_back(vertex[0]);
				out_vertices.push_back(vertex[1]);
				out_vertices.push_back(vertex[2]);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				unsigned int index1, index2, index3;
				fscanf(file, "%d %d %d\n", &index1, &index2, &index3);
				out_faces.push_back(index1 - 0);
				out_faces.push_back(index2 - 0);
				out_faces.push_back(index3 - 0);
			}
		}
		//printf("Finised loading OBJ file %s...\n", path.c_str());
		return true;
	}


	bool loadOBJOpenCV(string path, cv::Mat1d & out_vertices, cv::Mat1i & out_faces)
	{
		out_vertices.release();
		out_faces.release();
		//printf("Loading OBJ file %s...\n", path.c_str());

		FILE * file = fopen(path.c_str(), "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}

		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) {
				double vertexData[3];
				fscanf(file, "%lf %lf %lf\n", &vertexData[0], &vertexData[1], &vertexData[2]);
				cv::Mat1d vertex = cv::Mat1d(1, 3, vertexData);
				out_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				int faceData[3];
				fscanf(file, "%d %d %d\n", &faceData[0], &faceData[1], &faceData[2]);
				cv::Mat1i face = cv::Mat1i(1, 3, faceData);
				out_faces.push_back(face);
			}
		}
		//printf("Finised loading OBJ file %s...\n", path.c_str());
		return true;
	}	

public:
	unsigned int VAO, VBO, EBO; //are public to let opengl change them

	void loadMesh(string fileName)
	{
		this->vertices.clear();
		this->indices.clear();
		loadOBJ(fileName, this->vertices, this->indices);
		loadOBJOpenCV(fileName, this->verticesOpenCV, this->indicesOpenCV);
	}

	Mesh(string modelName, string fileName, ModelParameters params)
	{
		this->modelName = modelName;
		this->fileName = fileName;
		this->params = params;
		modelMatrix = glm::mat4(1.0f);
		loadMesh(fileName);
	}

	void updateModelMatrixByUserParameters()
	{
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(identityMatrix, glm::radians(params.angleX), glm::vec3(1.0, 0.0, 0.0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(params.angleY), glm::vec3(0.0, 1.0, 0.0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(params.angleZ), glm::vec3(0.0, 0.0, 1.0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(params.translateX, params.translateY, params.translateZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(params.scaleX, params.scaleY, params.scaleZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(params.scaleUniform, params.scaleUniform, params.scaleUniform));
	}

	void saveBuffersForRedneringWholeMesh()
	{
		glGenVertexArrays(1, &(this->VAO));
		glGenBuffers(1, &(this->VBO));
		glGenBuffers(1, &(this->EBO));
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->getVertices().size() * sizeof(float), &(this->getVertices())[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndices().size() * sizeof(unsigned int), &(this->getIndices())[0], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void renderObject()
	{
		GLenum err011 = glGetError();
		if (err011 != GL_NO_ERROR)
		{
			std::cout << "err011" << std::endl;
			printOpenGLError(err011);
		}
		glBindVertexArray(this->VAO);
		GLenum err012 = glGetError();
		if (err012 != GL_NO_ERROR)
		{
			std::cout << "err012" << std::endl;
			printOpenGLError(err012);
		}

		glDrawElements(GL_TRIANGLES, (GLsizei)this->getIndices().size(), GL_UNSIGNED_INT, 0);
		GLenum err013 = glGetError();
		if (err013 != GL_NO_ERROR)
		{
			std::cout << "err013" << std::endl;
			printOpenGLError(err013);
		}

		glBindVertexArray(0);
		GLenum err014 = glGetError();
		if (err014 != GL_NO_ERROR)
		{
			std::cout << "err014" << std::endl;
			printOpenGLError(err014);
		}

	}

	glm::mat4 getModelMatrix()
	{
		return modelMatrix;
	}

	vector<unsigned int> getIndices()
	{
		return indices;
	}

	vector<float> getVertices()
	{
		return vertices;
	}

	cv::Mat1i getIndicesOpenCV() {
		return indicesOpenCV;
	}

	cv::Mat1d getVerticesOpenCV() {
		return verticesOpenCV;
	}

	string getModelName() {
		return modelName;
	}

	ModelParameters getParams() {
		return params;
	}

	void setParams(ModelParameters params) {
		this->params = params;
	}

	cv::Mat1d calcRotatedVertices(cv::Mat1d rotation)
	{
		cv::Mat1d transposedVertices = this->verticesOpenCV.t();
		cv::Mat1d transposedRotatedVertices = rotation * transposedVertices;
		cv::Mat1d rotatedVertices = transposedRotatedVertices.t();
		return rotatedVertices;
	}


};

