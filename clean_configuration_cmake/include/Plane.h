#include <vector>
#include <opencv2/opencv.hpp>

class Plane {
private:
    cv::Vec3d _point;
    cv::Vec3d _unitPlaneNormal;

public:
    Plane(cv::Vec3d point, cv::Vec3d planeNormal) {
        this->_point = point;
        cv::normalize(planeNormal, this->_unitPlaneNormal , 1.0, 0.0, NORM_L2);
    }

    const cv::Vec3d point() const 
    {
        return _point;
    }

    void point(const cv::Vec3d& point)
    {
        _point = point;
    }


    const cv::Vec3d unitPlaneNormal() const 
    {
        return _unitPlaneNormal;
    }

    void unitPlaneNormal(const cv::Vec3d& unitPlaneNormal)
    {
        _unitPlaneNormal = unitPlaneNormal;
    }    


};