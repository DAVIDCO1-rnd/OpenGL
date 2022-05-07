#include <vector>
#include <string>
#include "ModelParameters.h"
#include <glad/glad.h>
#include <iostream>
#include "OpenGLErrors.h"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "Model.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


using namespace std;


# define PI           3.14159265358979323846  /* pi */

class CircleModel : public Model {
private:
    vector<float> _verticesInModelCoordinates;
    cv::Mat1d _verticesOpenCVInModelCoordinates;
    vector<unsigned int> _indices;
    cv::Mat1i _indicesOpenCV;

public:
    unsigned int VAO, VBO, EBO; //are public to let opengl change them

    CircleModel(string modelName, ModelParameters params, int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal);
    void generateBuffers();
    void saveBuffersForRedneringPolygones();
    void Draw(Shader& shader, Camera& camera);
    std::vector<float> BuildVerticesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal);
    std::vector<unsigned int> BuildTrianglesIndicesForCircleTriangulation(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal);
    void convertVerticesToMat1d();
    void convertIndicesToMat1i();
    void createMesh(int numOfAngles, float radius, float minRadius, int numOfRadiuses, float circleCenterX, float circleCenterY, float zVal);
    vector<unsigned int> indices() const;
    void indicesWithLos(vector<unsigned int> indices);
};