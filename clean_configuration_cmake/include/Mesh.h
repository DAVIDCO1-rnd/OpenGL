#pragma once
#include "Object.h"
#include <vector>
#include "ModelParameters.h"
#include "OpenGLErrors.h"
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"

#include "MatrixDebugger.h"
#include "MathUseful.h"

using std::vector;


class Mesh
{
protected:
	string modelName;
	string fileName;
	vector<unsigned int> indices;
	vector<float> verticesInModelCoordinates;
	//vector<float> verticesInWorldCoordinates;

	vector<glm::vec3> _verticesInModelCoordinatesGLM;
	vector<glm::vec3> _verticesInWorldCoordinatesGLM;

	cv::Mat1i indicesOpenCV;

	cv::Mat1d _verticesOpenCVInModelCoordinates;
	cv::Mat1d _verticesOpenCVInWorldCoordinates; 
	
	cv::Mat _modelMatrixCV;

	glm::mat4 modelMatrix;
	ModelParameters params;

	glm::vec3 convertVertexFromModelToWorldCoords(glm::vec3 vertexInModelCoords)
	{
		glm::vec4 vertexInModelCoords4d = glm::vec4(vertexInModelCoords, 1.0f);
		glm::vec4 vertexInWorldCoords4d = this->modelMatrix * vertexInModelCoords4d;
		glm::vec3 vertexInWorldCoords = glm::vec3(vertexInWorldCoords4d[0], vertexInWorldCoords4d[1], vertexInWorldCoords4d[2]);
		return vertexInWorldCoords;
	}

	void convertModelCoordsToWorldCoords()
	{
		int numOfVertices = this->_verticesInModelCoordinatesGLM.size();
		for (int i=0 ; i<numOfVertices ; i++) {
			this->_verticesInWorldCoordinatesGLM[i] = convertVertexFromModelToWorldCoords(this->_verticesInModelCoordinatesGLM[i]);
		}
	}

	bool loadOBJ(string path, vector<unsigned int>& out_faces)
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
				this->verticesInModelCoordinates.push_back(vertex[0]);
				this->verticesInModelCoordinates.push_back(vertex[1]);
				this->verticesInModelCoordinates.push_back(vertex[2]);
				glm::vec3 vertexInModelCoords(vertex[0], vertex[1], vertex[2]);
				glm::vec3 vertexInWorldCoords = convertVertexFromModelToWorldCoords(vertexInModelCoords);

				this->_verticesInModelCoordinatesGLM.push_back(vertexInModelCoords);
				this->_verticesInWorldCoordinatesGLM.push_back(vertexInWorldCoords);
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
		this->verticesInModelCoordinates.clear();
		this->indices.clear();
		loadOBJ(fileName, this->indices);
		loadOBJOpenCV(fileName, this->_verticesOpenCVInModelCoordinates, this->indicesOpenCV);
	}	

	Mesh(string modelName, string fileName, ModelParameters params)
	{
		this->modelName = modelName;
		this->fileName = fileName;
		this->params = params;
		this->modelMatrix = glm::mat4(1.0f);
		this->_modelMatrixCV = cv::Mat::eye(4, 4, CV_64F);
		updateModelMatrixByUserParameters();
		loadMesh(fileName);
	}

	cv::Mat1d calcAverageOfAxises() {
		cv::Mat1d rowMean;
		cv::reduce(this->_verticesOpenCVInModelCoordinates, rowMean, 0, CV_REDUCE_AVG);
		std::cout << this->modelName << std::endl;
		MatrixDebugger::printMatrix("rowMean", rowMean);
		return rowMean;	
	}

	void updateModelMatrixByUserParameters()
	{
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		this->modelMatrix = glm::rotate(identityMatrix, glm::radians(params.angleX), glm::vec3(1.0, 0.0, 0.0));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(params.angleY), glm::vec3(0.0, 1.0, 0.0));
		this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(params.angleZ), glm::vec3(0.0, 0.0, 1.0));
		this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(params.translateX, params.translateY, params.translateZ));
		this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(params.scaleX, params.scaleY, params.scaleZ));
		this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(params.scaleUniform, params.scaleUniform, params.scaleUniform));

		MathUseful mathUseful;
		bool success = mathUseful.fromGLM2CV(this->modelMatrix, this->_modelMatrixCV);
		if (success == false) {
			throw "convertion from GLM to CV failed!";
		}
		//MatrixDebugger::printMatrix("this->_modelMatrixCV", this->_modelMatrixCV);
	}

	void saveBuffersForRedneringWholeMesh()
	{
		glGenVertexArrays(1, &(this->VAO));
		glGenBuffers(1, &(this->VBO));
		glGenBuffers(1, &(this->EBO));
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->verticesInModelCoordinates.size() * sizeof(float), &(this->verticesInModelCoordinates)[0], GL_STATIC_DRAW);
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

	vector<float> getVerticesInModelCoordinates()
	{
		return verticesInModelCoordinates;
	}

	vector<glm::vec3> getVerticesInWorldCoordinatesGLM()
	{
		convertModelCoordsToWorldCoords();
		return this->_verticesInWorldCoordinatesGLM;
	}

	cv::Mat1i getIndicesOpenCV() {
		return indicesOpenCV;
	}

	cv::Mat1d getVerticesOpenCVInModelCoordinates() {
		return this->_verticesOpenCVInModelCoordinates;
	}

	cv::Mat1d calculateVerticesOpenCVInWorldCoordinates() {
		cv::Mat ones = cv::Mat::ones(this->_verticesOpenCVInModelCoordinates.rows, 1, CV_64F);
		cv::Mat matArray[] = { this->_verticesOpenCVInModelCoordinates, ones};		
		cv::Mat verticesOpenCVInModelCoordinates4d;
		cv::hconcat(matArray, 2, verticesOpenCVInModelCoordinates4d);
		cv::Mat verticesOpenCVInModelCoordinates4dTransposed = verticesOpenCVInModelCoordinates4d.t();
		//MatrixDebugger::printMatrix("this->_modelMatrixCV", this->_modelMatrixCV);
		cv::Mat verticesOpenCVInWorldCoordinates4dTransposed = this->_modelMatrixCV * verticesOpenCVInModelCoordinates4dTransposed;
		cv::Mat verticesOpenCVInWorldCoordinates4d = verticesOpenCVInWorldCoordinates4dTransposed.t();
		this->_verticesOpenCVInWorldCoordinates = verticesOpenCVInWorldCoordinates4d.colRange(0,3);
		//MatrixDebugger::printMatrix("this->verticesOpenCVInWorldCoordinates", this->verticesOpenCVInWorldCoordinates);
		return this->_verticesOpenCVInWorldCoordinates;
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
		this->_verticesOpenCVInWorldCoordinates = calculateVerticesOpenCVInWorldCoordinates();
		cv::Mat1d transposedVertices = this->_verticesOpenCVInWorldCoordinates.t();
		cv::Mat1d transposedRotatedVertices = rotation * transposedVertices;
		cv::Mat1d rotatedVertices = transposedRotatedVertices.t();
		return rotatedVertices;
	}


};

