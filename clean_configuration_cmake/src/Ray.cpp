#include "Ray.h"

Ray::Ray(cv::Vec3d point, cv::Vec3d vec) {
    this->_point = point;
    this->_unitVec = cv::normalize(vec);
}

Ray::Ray(const Ray &ray) {
    this->_point = ray._point;
    this->_unitVec = ray._unitVec;
}

Ray& Ray::operator=(Ray otherRay) {
    this->_point = otherRay._point;
    this->_unitVec = otherRay._unitVec;
    return *this;
}

Ray Ray::calcRotatedRay(cv::Mat rotation) {
    cv::Mat rotatedPointMat = rotation * cv::Mat(this->_point);
    cv::Mat rotatedUnitVecMat = rotation * cv::Mat(this->_unitVec);

    cv::Vec3d rotatedPoint = (cv::Vec3d)rotatedPointMat;
    cv::Vec3d rotatedUnitVec = (cv::Vec3d)rotatedUnitVecMat;
    
    Ray rotatedRay(rotatedPoint, rotatedUnitVec);
    return rotatedRay;
}

cv::Vec3d Ray::point() const {
    return _point;
}

cv::Vec3d Ray::unitVec() const {
    return _unitVec;
}