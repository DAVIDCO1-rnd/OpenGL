#include "MathUseful.h"

bool MathUseful::checkIfNumbersAreEqual(double num1, double num2)
{
	bool isEqual = false;
	double diff_numbers = num2 - num1;
	double sqr_diff_numbers = std::pow(diff_numbers, 2);
	if (sqr_diff_numbers < std::numeric_limits<double>::epsilon())
	{
		isEqual = true;
	}
	return isEqual;
}

bool MathUseful::checkIfPointsAreEqual(Vec3d point1, Vec3d point2)
{
	bool areEqual = false;
	Vec3d diffPoints = point2 - point1;
	double normDiffPoints = norm(diffPoints);
	if (normDiffPoints < std::numeric_limits<double>::epsilon())
	{
		areEqual = true;
	}

	return areEqual;
}

void MathUseful::calcNullSpace(Vec3d unitVec, Vec3d& unitU, Vec3d& unitW)
{
    bool areVectorsEqual = checkIfPointsAreEqual(unitVec, Vec3d{ 0,0,1 });
    if (areVectorsEqual)
    {
        unitU[0] = 1;
        unitU[1] = 0;
        unitU[2] = 0;

        unitW[0] = 0;
        unitW[1] = 1;
        unitW[2] = 0;
    }
    else
    {
        Vec3d u = Vec3d(unitVec[1], -unitVec[0], 0.0);
        unitU = normalizeVec(u);

        Vec3d w = unitVec.cross(unitU);
        unitW = normalizeVec(w);
    }
}


cv::Mat MathUseful::calcRotation(cv::Vec3d unitVec)
{
    cv::Mat rotation;
    cv::Vec3d unitU;
    cv::Vec3d unitW;
    calcNullSpace(unitVec, unitU, unitW);
    cv::Mat v(1, 3, CV_64FC1);
    v.at<double>(0, 0) = unitVec[0];
    v.at<double>(0, 1) = unitVec[1];
    v.at<double>(0, 2) = unitVec[2];

    cv::Mat u(1, 3, CV_64FC1);
    u.at<double>(0, 0) = unitU[0];
    u.at<double>(0, 1) = unitU[1];
    u.at<double>(0, 2) = unitU[2];

    cv::Mat w(1, 3, CV_64FC1);
    w.at<double>(0, 0) = unitW[0];
    w.at<double>(0, 1) = unitW[1];
    w.at<double>(0, 2) = unitW[2];

    vconcat(u, w, rotation);
    vconcat(rotation, v, rotation);

    double det = cv::determinant(rotation);

    if (checkIfNumbersAreEqual(det, -1))
    {
        Mat rotation1;
        vconcat(-u, w, rotation1);
        vconcat(rotation1, v, rotation1);
        return rotation1;
    }

    return rotation;
}

Vec3d MathUseful::normalizeVec(Vec3d vec)
{
	double normVec = norm(vec);
	Vec3d unitVec;
	unitVec[0] = vec[0] / normVec;
	unitVec[1] = vec[1] / normVec;
	unitVec[2] = vec[2] / normVec;
	return unitVec;
}

// bool MathUseful::fromCV2GLM(const cv::Mat& cvmat, glm::mat4* glmmat) {
//     if (cvmat.cols != 4 || cvmat.rows != 4 || cvmat.type() != CV_64F) {
//         std::cout << "Mat_CV2GLM Matrix conversion error!" << std::endl;
//         return false;
//     }
//     memcpy(glm::value_ptr(*glmmat), cvmat.data, 16 * sizeof(double));
//     *glmmat = glm::transpose(*glmmat);
//     return true;
// }

bool MathUseful::fromGLM2CV(const glm::mat4& glmmat, cv::Mat cvmat) {

    for (int i=0 ; i<4 ; i++)
    {
        for (int j=0 ; j<4 ; j++)
        {
            double val = glmmat[i][j];
            cvmat.at<double>(i,j) = val;
        }
    }
    cvmat = cvmat.t();
    return true;
}

glm::vec3 MathUseful::getRotationAngles(glm::mat4 rotation_matrix) {
	float R13 = rotation_matrix[0][2];
	float R12 = rotation_matrix[0][1];
	float R11 = rotation_matrix[0][0];
	float R23 = rotation_matrix[1][2];
	float R33 = rotation_matrix[2][2];
	float angleRadX, angleRadY, angleRadZ;
	if (R13 == 1 || R13 == -1)
	{
		//special case
		angleRadZ = 0.0f; //set arbitrarily
		float dlta = atan2(R12, R13);


		if (R13 == -1)
		{
			angleRadY = PI / 2.0;
			angleRadX = angleRadZ + dlta;
		}					
		else
		{
			angleRadY = -PI / 2.0;
			angleRadX = -angleRadZ + dlta;
		}
	}
	else
	{
		angleRadY = -asin(R13);
		angleRadX = atan2(R23 / cos(angleRadY), R33 / cos(angleRadY));
		angleRadZ = atan2(R12 / cos(angleRadY), R11 / cos(angleRadY));
	}	
	float angleDegX = angleRadX * 180.0 / PI;
	float angleDegY = angleRadY * 180.0 / PI;
	float angleDegZ = angleRadZ * 180.0 / PI;
	glm::vec3 anglesDeg = glm::vec3(angleDegX, angleDegY, angleDegZ);
	return anglesDeg;
}