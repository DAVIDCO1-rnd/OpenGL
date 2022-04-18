
#include <opencv2/opencv.hpp>

class Ray {
private:
    cv::Vec3d _point;
    cv::Vec3d _unitVec;    

public:
    Ray(cv::Vec3d point, cv::Vec3d vec);

    Ray(const Ray &ray);
    Ray& operator=(Ray otherRay);
    Ray calcRotatedRay(cv::Mat rotation);

    cv::Vec3d point() const;
    cv::Vec3d unitVec() const;


};