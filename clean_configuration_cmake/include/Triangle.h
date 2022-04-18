#include "opencv2/opencv.hpp"


class Triangle {
private:
    cv::Vec3d _point1;
    cv::Vec3d _point2;
    cv::Vec3d _point3;
    cv::Vec3d _triangleUnitNormal;

    cv::Vec3d calcTriangleUnitNormal() {
        cv::Vec3d vec1 = this->_point2 - this->_point1;
        cv::Vec3d vec2 = this->_point3 - this->_point1;
        cv::Vec3d triangleNormal = vec1.cross(vec2);

        cv::Vec3d unitTriangleNormal;
        cv::normalize(triangleNormal, unitTriangleNormal, 1.0, 0.0, NORM_L2); 
        return unitTriangleNormal;         
    }

public:
    Triangle(cv::Vec3d point1, cv::Vec3d point2, cv::Vec3d point3) {
        this->_point1 = point1;
        this->_point2 = point2;
        this->_point3 = point3;
        this->_triangleUnitNormal = calcTriangleUnitNormal();
    }

    Triangle calcRotatedTriangle(cv::Mat1d rotation) {
        cv::Mat1d pointRotatedMat1 = rotation * cv::Mat1d(this->_point1);
        cv::Mat1d pointRotatedMat2 = rotation * cv::Mat1d(this->_point2);
        cv::Mat1d pointRotatedMat3 = rotation * cv::Mat1d(this->_point3);

        cv::Vec3d pointRotated1(pointRotatedMat1.reshape(3).at<cv::Vec3d>());
        cv::Vec3d pointRotated2(pointRotatedMat2.reshape(3).at<cv::Vec3d>()); 
        cv::Vec3d pointRotated3(pointRotatedMat3.reshape(3).at<cv::Vec3d>());

        Triangle rotatedTriangle(pointRotated1, pointRotated2, pointRotated3);
        return rotatedTriangle;     
    }

    cv::Vec3d point1() const
    {
        return this->_point1;
    }

    cv::Vec3d point2() const
    {
        return this->_point2;
    }

    cv::Vec3d point3() const
    {
        return this->_point3;
    } 

    cv::Vec3d triangleUnitNormal() const
    {
        return this->_triangleUnitNormal;
    }            

};