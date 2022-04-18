
#include <opencv2/opencv.hpp>

using namespace cv;

class MathUseful {
private:


public:
    bool checkIfNumbersAreEqual(double num1, double num2);
    bool checkIfPointsAreEqual(Vec3d point1, Vec3d point2);
    void calcNullSpace(Vec3d unitVec, Vec3d& unitU, Vec3d& unitW);
    Mat calcRotation(Vec3d unitVec);
    Vec3d normalizeVec(Vec3d vec);
    template <typename T> int sign(T num)
    {
        if (num > 0) return 1;
        if (num < 0) return -1;
        return 0;
    }
};