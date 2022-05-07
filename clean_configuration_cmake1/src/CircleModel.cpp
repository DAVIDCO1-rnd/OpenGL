#include "CircleModel.h"





CircleModel::CircleModel(string modelName, ModelParameters params, int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) : Model(modelName, params) {
    createMesh(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
}


void CircleModel::generateBuffers() {
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &(this->VBO));
	glGenBuffers(1, &(this->EBO));             
}


void CircleModel::saveBuffersForRedneringPolygones()
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


void CircleModel::Draw(Shader& shader, Camera& camera)
{
    //Camera is not used (for now). I sent it as a parameter only to make it virtual method with the same parameters as the base
    shader.Activate();
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


std::vector<float> CircleModel::BuildVerticesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) {
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

std::vector<unsigned int> CircleModel::BuildTrianglesIndicesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal)
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

void CircleModel::convertVerticesToMat1d() {
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

void CircleModel::convertIndicesToMat1i() {
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

void CircleModel::createMesh(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal) {
    this->_verticesInModelCoordinates = BuildVerticesForCircleTriangulation(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
    this->_indices = BuildTrianglesIndicesForCircleTriangulation(numOfAngles, radius, minRadius, numOfRadiuses, circleCenterX, circleCenterY, zVal);
    convertVerticesToMat1d();
    convertIndicesToMat1i();        
}

vector<unsigned int> CircleModel::indices() const {
    return this->_indices;
}

void CircleModel::indicesWithLos(vector<unsigned int> indices) {
    this->_indices = indices;
}  

