
#pragma once
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> //for glm::value_ptr

//using namespace cv;

#define PI 3.14159265358979323846

class MathUseful {
private:


public:
    bool checkIfNumbersAreEqual(double num1, double num2);
    bool checkIfPointsAreEqual(cv::Vec3d point1, cv::Vec3d point2);
    void calcNullSpace(cv::Vec3d unitVec, cv::Vec3d& unitU, cv::Vec3d& unitW);
    cv::Mat calcRotation(cv::Vec3d unitVec);
    cv::Vec3d normalizeVec(cv::Vec3d vec);
    template <typename T> int sign(T num)
    {
        if (num > 0) return 1;
        if (num < 0) return -1;
        return 0;
    }

    //bool fromCV2GLM(const cv::Mat& cvmat, glm::mat4* glmmat);
    bool fromGLM2CV(const glm::mat4& glmmat, cv::Mat cvmat);

	glm::vec3 getRotationAngles(glm::mat4 rotationMatrix);
};