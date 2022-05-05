#include <vector>
#include <string>
#include "ModelParameters.h"
#include <glad/glad.h>
#include <iostream>
#include "OpenGLErrors.h"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"

//#include<glm/glm.hpp>
//#include<glm/gtc/matrix_transform.hpp>
//#include<glm/gtc/type_ptr.hpp>
//#include<glm/gtx/rotate_vector.hpp>
//#include<glm/gtx/vector_angle.hpp>


using namespace std;


# define PI           3.14159265358979323846  /* pi */

class CircleMesh {
private:
    vector<float> _verticesInModelCoordinates;
    cv::Mat1d _verticesOpenCVInModelCoordinates;
    vector<unsigned int> _indices;
    cv::Mat1i _indicesOpenCV;
    string _modelName;
    ModelParameters _params;
    glm::mat4 _modelMatrix;

public:
    unsigned int VAO, VBO, EBO; //are public to let opengl change them

    ////CircleMesh(string modelName, ModelParameters params) {
    ////    int numOfAngles = 36;
    ////    int numOfRadiuses = 2;
    ////    float radius = 100.0f;
    ////    float minRadius = 50.0f;        
    ////    float circleCenterX = 0.0f;
    ////    float circleCenterY = 0.0f;
    ////    float zVal = 200.0f;        
    ////    createMesh(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
    ////}

    CircleMesh(string modelName, ModelParameters params, int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) {
        this->_modelName = modelName;
        this->_params = params;
        createMesh(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
    }


    void generateBuffers() {
		glGenVertexArrays(1, &(this->VAO));
		glGenBuffers(1, &(this->VBO));
		glGenBuffers(1, &(this->EBO));             
    }


	void saveBuffersForRedneringPolygones()
	{
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->_verticesInModelCoordinates.size() * sizeof(float), &(this->_verticesInModelCoordinates)[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &(_indices)[0], GL_STATIC_DRAW);
	
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	} 


	void renderPolygones()
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

        glDrawElements(GL_TRIANGLES, (GLsizei)_indices.size(), GL_UNSIGNED_INT, 0);
		    
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


    std::vector<float> BuildVerticesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) {
        int numOfVertices = numOfAngles * numOfRadiuses + 1; //plus 1 for the center itself
        std::vector<float> vertices(numOfVertices * 3, 0.0f);

        size_t vertexIndex = -1;
        for (int radiusIndex = numOfRadiuses - 1 ; radiusIndex >=0 ; radiusIndex--) {
            for (int angleIndex = 0 ; angleIndex < numOfAngles ; angleIndex++) {
                float currentRadius = minRadius + radiusIndex * (radius - minRadius) / (numOfRadiuses - 1);
                float currentAngle = angleIndex * 2 * PI / numOfAngles;

                float currentVertexX = circleCenterX + currentRadius * cos(currentAngle);
                float currentVertexY = circleCenterY + currentRadius * sin(currentAngle);

                vertexIndex++;
                vertices[3*vertexIndex + 0] = currentVertexX;
                vertices[3*vertexIndex + 1] = currentVertexY;
                vertices[3*vertexIndex + 2] = zVal;
            }
        }
        vertexIndex++;
        vertices[3*vertexIndex + 0] = circleCenterX;
        vertices[3*vertexIndex + 1] = circleCenterY;
        vertices[3*vertexIndex + 2] = zVal;
        return vertices;
    }

    std::vector<unsigned int> BuildTrianglesIndicesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal)
    {
        int numOfVertices = numOfAngles * numOfRadiuses + 1; //plus 1 for the center itself
        int numOfTriangles = numOfAngles * 2 * (numOfRadiuses - 1) + numOfAngles;
        std::vector<unsigned int> triangles(3 * numOfTriangles, 0);

        int index = 0;
        for (int radiusIndex = 0 ; radiusIndex < numOfRadiuses - 1 ; radiusIndex++) {
            for (int angleIndex = 0 ; angleIndex < numOfAngles ; angleIndex++) {
                int index2 = index + 1;
                if (index2 == (radiusIndex + 1) * numOfAngles)
                {
                    index2 = (radiusIndex - 0) * numOfAngles;
                }
                int face1Index1 = index;
                int face1Index2 = index2;
                int face1Index3 = index + numOfAngles;

                int index3 = index + numOfAngles + 1;
                if (index3 == (radiusIndex + 2) * numOfAngles)
                {
                    index3 = (radiusIndex + 1) * numOfAngles;
                }
                int face2Index1 = index2;
                int face2Index2 = index + numOfAngles;
                int face2Index3 = index3; 

                triangles[6*index + 0] = face1Index1;
                triangles[6*index + 1] = face1Index2;
                triangles[6*index + 2] = face1Index3;

                triangles[6*index + 3] = face2Index1;
                triangles[6*index + 4] = face2Index2;
                triangles[6*index + 5] = face2Index3;            

                index++;         
            }
        }

        int firstIndex = (numOfRadiuses - 1) * numOfAngles;
        index = firstIndex; 
        int counter = 0;
        int indexOfCenter = numOfVertices - 1;
        for (int angleIndex = 0 ; angleIndex < numOfAngles ; angleIndex++) {
            int index2 = index + 1;
            if (index2 == indexOfCenter)
            {
                index2 = firstIndex;
            }
            int faceIndex1 = index;
            int faceIndex2 = index2;
            int faceIndex3 = indexOfCenter;

            triangles[6 * firstIndex + 3 * counter + 0] = faceIndex1;
            triangles[6 * firstIndex + 3 * counter + 1] = faceIndex2;
            triangles[6 * firstIndex + 3 * counter + 2] = faceIndex3; 
            index++; 
            counter++;        
        }  

        return triangles;
    }

    void convertVerticesToMat1d() {
        int numOfVertices = this->_verticesInModelCoordinates.size() / 3;
        double vertexData[3];
        this->_verticesOpenCVInModelCoordinates.release();
        for (int i=0 ; i < numOfVertices ; i++)
        {            
            vertexData[0] = this->_verticesInModelCoordinates[3*i + 0];
            vertexData[1] = this->_verticesInModelCoordinates[3*i + 1];
            vertexData[2] = this->_verticesInModelCoordinates[3*i + 2];
            cv::Mat1d vertex = cv::Mat1d(1, 3, vertexData);
            this->_verticesOpenCVInModelCoordinates.push_back(vertex);            
        }
    }

    void convertIndicesToMat1i() {
        int numOfTriangles = this->_indices.size() / 3;
        int faceData[3];
        this->_indicesOpenCV.release();
        for (int i=0 ; i < numOfTriangles ; i++)
        {
            faceData[0] = this->_indices[3*i + 0];
            faceData[1] = this->_indices[3*i + 1];
            faceData[2] = this->_indices[3*i + 2];
            cv::Mat1i face = cv::Mat1i(1, 3, faceData);
            this->_indicesOpenCV.push_back(face);            
        }
    }    

    void createMesh(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) {
        this->_verticesInModelCoordinates = BuildVerticesForCircleTriangulation(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
        this->_indices = BuildTrianglesIndicesForCircleTriangulation(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
        convertVerticesToMat1d();
        convertIndicesToMat1i();        
    }

    void updateModelMatrixByUserParameters()
    {
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        this->_modelMatrix = glm::rotate(identityMatrix, glm::radians(this->_params.angleZ), glm::vec3(0.0, 0.0, 1.0));
        this->_modelMatrix = glm::rotate(this->_modelMatrix, glm::radians(this->_params.angleY), glm::vec3(0.0, 1.0, 0.0));
        this->_modelMatrix = glm::rotate(this->_modelMatrix, glm::radians(this->_params.angleX), glm::vec3(1.0, 0.0, 0.0));
        this->_modelMatrix = glm::translate(this->_modelMatrix, glm::vec3(this->_params.translateX, this->_params.translateY, this->_params.translateZ));
        this->_modelMatrix = glm::scale(this->_modelMatrix, glm::vec3(this->_params.scaleX, this->_params.scaleY, this->_params.scaleZ));
        this->_modelMatrix = glm::scale(this->_modelMatrix, glm::vec3(this->_params.scaleUniform, this->_params.scaleUniform, this->_params.scaleUniform));
    }

    glm::mat4 getModelMatrix()
    {
        return _modelMatrix;
    }

    vector<unsigned int> indices() const {
        return this->_indices;
    }

    void indicesWithLos(vector<unsigned int> indices) {
        this->_indices = indices;
    }  

    std::string getModelName() {
        return _modelName;
    }
};