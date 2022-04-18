
#include <opencv2/opencv.hpp>
#include "Mesh.h"
#include "MathUseful.h"
#include "Ray.h"
#include <iostream>
#include <vector>
#include "Plane.h"
#include "Triangle.h"


class RayMesh {
private:
    // cv::Vec3d point;
    // cv::Vec3d unitVec;
    Ray ray;

public:
    // RayMesh(cv::Vec3d point, cv::Vec3d vec) {
    //     this->point = point;
    //     this->unitVec = cv::normalize(vec);
    // }

    RayMesh(Ray otherRay) : ray(otherRay) {
    }  

    void printMatrix(const char* matrixName, cv::Mat matrix) {
        std::cout << matrixName << std::endl;
        std::cout << matrix << std::endl;
        std::cout << "\n\n";  
    }

    // cv::Mat1i extractValuesFromIndexes(cv::Mat1i vals, cv::Mat1i indexes) {
    //     cv::Size size = indexes.size();
    //     cv::Mat1i newVals(size);

    //     for (int i=0 ; i < indexes.rows ; i++)
    //     {
    //         int currentIndex = indexes.at<int>(i);
    //         int currentVal = vals.at<int>(currentIndex);
    //         newVals.at<int>(i) = currentVal;
    //     }

    //     return newVals;
    // }


    // cv::Mat1i calcTrianglesWithAllVerticesBiggerThanLosX(cv::Mat1i vals, cv::Mat1i facesIndexes) {
    //     int rows = facesIndexes.rows;
    //     int cols = facesIndexes.cols;

    //     cv::Mat1i newVals(rows, 1);

    //     for (int i=0 ; i < rows ; i++)
    //     {
    //         int currentVal = 1;
    //         for (int j=0 ; j < cols ; j++)
    //         {
    //             int currentIndex = facesIndexes.at<int>(i, j);
    //             int currentVertexVal = vals.at<int>(currentIndex);
    //             if (currentVertexVal == 0)
    //             {
    //                 currentVal = 0;
    //                 continue;
    //             }
    //         }
    //         newVals.at<int>(i) = currentVal;
    //     }

    //     return newVals;
    // } 


    std::vector<int> calcTrianglesThatMightIntersectRay(cv::Mat1d rotatedVertices, Ray rayParallelToZ, cv::Mat1i trianglesIndexes) {
        int numOfTriangles = trianglesIndexes.rows;
        cv::Mat1i trianglesThatMightIntersectRay(numOfTriangles, 1);

        double rayPointX = rayParallelToZ.point()[0];
        double rayPointY = rayParallelToZ.point()[1];

        std::vector<int> trianglesIndexesThatMightIntersectRay;

        for (int triangleIndex = 0 ; triangleIndex < numOfTriangles ; triangleIndex++)
        {
            int biggerX = 0;
            int biggerY = 0;
            int smallerX = 0;
            int smallerY = 0;

            for (int j = 0 ; j < 3 ; j++) //for j=0 -> x val, for j=1 -> y val, and for j=2 -> z val
            {
                int currentIndex = trianglesIndexes.at<int>(triangleIndex, j);                
                
                double currentVertexX = rotatedVertices.at<double>(currentIndex, 0);
                double currentVertexY = rotatedVertices.at<double>(currentIndex, 1);

                if (currentVertexX > rayPointX)
                {
                    biggerX++;
                }

                if (currentVertexY > rayPointY)
                {
                    biggerY++;
                }  

                if (currentVertexX < rayPointX)
                {
                    smallerX++;
                }

                if (currentVertexY < rayPointY)
                {
                    smallerY++;
                }
            }

            if (biggerX < 3 && biggerY < 3 && smallerX < 3 && smallerY < 3)
            {
                trianglesIndexesThatMightIntersectRay.push_back(triangleIndex);
            }
            
        }

        return trianglesIndexesThatMightIntersectRay;
    }

    bool calcIntersectionBetweenRayAndPlane(Ray ray, Plane plane, cv::Vec3d& pointIntersectionBetweenRayAndPlane, double& t_intersection) {
        bool isIntersecting = false;
        t_intersection = -1;
        cv::Vec3d q1 = ray.point();
        cv::Vec3d q2 = ray.point() + ray.unitVec();
        cv::Vec3d p1 = plane.point();
        cv::Vec3d n = plane.unitPlaneNormal();
        double nominator = n.dot(q2 - q1);
        double absNominator = cv::abs(nominator);
        if (absNominator < std::numeric_limits<double>::epsilon())
        {
            throw "Division by zero";
        }
        double t = -n.dot(q1-p1) / nominator;

        if (t > 0) //since this is a ray and not a line
        {
            pointIntersectionBetweenRayAndPlane = q1 + t * (q2-q1);
            t_intersection = t;
            isIntersecting = true;
        }

        return isIntersecting;
    }

    double calcDeterminant(cv::Mat matrix) {
        double determinant = cv::determinant(matrix);
        if (cv::abs(determinant) < std::numeric_limits<double>::epsilon())
        {
            determinant = 0;
        }
        return determinant;
    }

    bool calcIfPointIsInsideTriangle(cv::Vec3d pointIntersectionBetweenRayAndPlane, Triangle triangle)
    {
        
        MathUseful mathUseful;
        cv::Mat rotation = mathUseful.calcRotation(triangle.triangleUnitNormal());

        Triangle triangleParallelToXyPlane = triangle.calcRotatedTriangle(rotation);
        cv::Mat1d rotatedPointIntersectionBetweenRayAndPlaneMat = rotation * cv::Mat1d(pointIntersectionBetweenRayAndPlane);
        cv::Vec3d rotatedPointIntersectionBetweenRayAndPlane(rotatedPointIntersectionBetweenRayAndPlaneMat.reshape(3).at<cv::Vec3d>());

        cv::Vec2d p1(triangleParallelToXyPlane.point1()[0], triangleParallelToXyPlane.point1()[1]);
        cv::Vec2d p2(triangleParallelToXyPlane.point2()[0], triangleParallelToXyPlane.point2()[1]);
        cv::Vec2d p3(triangleParallelToXyPlane.point3()[0], triangleParallelToXyPlane.point3()[1]);
        cv::Vec2d p(rotatedPointIntersectionBetweenRayAndPlane[0], rotatedPointIntersectionBetweenRayAndPlane[1]);

        cv::Vec2d p12 = p1 - p2;
        cv::Vec2d p23 = p2 - p3;
        cv::Vec2d p31 = p3 - p1;

        cv::Vec2d p1p = p1 - p;
        cv::Vec2d p2p = p2 - p;
        cv::Vec2d p3p = p3 - p;


        Mat mat11 = (Mat_<double>(2,2) << p31[0], p31[1], p23[0], p23[1]);
        Mat mat21 = (Mat_<double>(2,2) << p12[0], p12[1], p31[0], p31[1]);
        Mat mat31 = (Mat_<double>(2,2) << p23[0], p23[1], p12[0], p12[1]);

        Mat mat12 = (Mat_<double>(2,2) << p3p[0], p3p[1], p23[0], p23[1]);
        Mat mat22 = (Mat_<double>(2,2) << p1p[0], p1p[1], p31[0], p31[1]);
        Mat mat32 = (Mat_<double>(2,2) << p2p[0], p2p[1], p12[0], p12[1]);

        printMatrix("mat11", mat11);
        printMatrix("mat21", mat21);
        printMatrix("mat31", mat31);

        printMatrix("mat11", mat12);
        printMatrix("mat22", mat22);
        printMatrix("mat32", mat32);        

        double detMat11 = calcDeterminant(mat11);
        double detMat21 = calcDeterminant(mat21);
        double detMat31 = calcDeterminant(mat31);

        double detMat12 = calcDeterminant(mat12);
        double detMat22 = calcDeterminant(mat22);
        double detMat32 = calcDeterminant(mat32);

        int signDetMat11 = mathUseful.sign<double>(detMat11);
        int signDetMat21 = mathUseful.sign<double>(detMat21);
        int signDetMat31 = mathUseful.sign<double>(detMat31);

        int signDetMat12 = mathUseful.sign<double>(detMat12);
        int signDetMat22 = mathUseful.sign<double>(detMat22);
        int signDetMat32 = mathUseful.sign<double>(detMat32);


        bool isPointInsideTriangle = signDetMat11*signDetMat12 >= 0 && signDetMat21*signDetMat22 >= 0 && signDetMat31*signDetMat32 >= 0;
        return isPointInsideTriangle;

    }

    bool calcIntersectionBetweenRayAndSingleTriangle(Ray ray, Triangle triangle, double& t_intersection) {
        Plane plane(triangle.point1(), triangle.triangleUnitNormal());        
        cv::Vec3d pointIntersectionBetweenRayAndPlane;
        bool isIntersectingPlane = calcIntersectionBetweenRayAndPlane(ray, plane, pointIntersectionBetweenRayAndPlane, t_intersection);
        if (isIntersectingPlane == false)
        {
            return isIntersectingPlane;
        }

        bool isPointInsideTriangle = calcIfPointIsInsideTriangle(pointIntersectionBetweenRayAndPlane, triangle);
        return isPointInsideTriangle;
    }

    bool calcIntersectionBetweenRayAndMultipleTriangles(Ray ray, Mesh mesh, std::vector<int> trianglesIndexesThatMightIntersectRay, cv::Vec3d& intersectionPoint, double& min_t)
    {
        bool isIntersecting = false;
        min_t = -1;
        for (size_t i=0 ; i<trianglesIndexesThatMightIntersectRay.size() ; i++)
        {
            int currentTriangleIndex = trianglesIndexesThatMightIntersectRay[i];
            cv::Mat1i meshTriangles = mesh.getIndicesOpenCV();
            cv::Mat1i currentTriangleVerticesIndexes = meshTriangles.row(currentTriangleIndex);
            int vertexIndex1 = currentTriangleVerticesIndexes.at<int>(0);
            int vertexIndex2 = currentTriangleVerticesIndexes.at<int>(1);
            int vertexIndex3 = currentTriangleVerticesIndexes.at<int>(2);
            cv::Mat1d meshVertices = mesh.getVerticesOpenCV();

            cv::Vec3d point1(meshVertices.row(vertexIndex1).at<double>(0), meshVertices.row(vertexIndex1).at<double>(1), meshVertices.row(vertexIndex1).at<double>(2));
            cv::Vec3d point2(meshVertices.row(vertexIndex2).at<double>(0), meshVertices.row(vertexIndex2).at<double>(1), meshVertices.row(vertexIndex2).at<double>(2));
            cv::Vec3d point3(meshVertices.row(vertexIndex3).at<double>(0), meshVertices.row(vertexIndex3).at<double>(1), meshVertices.row(vertexIndex3).at<double>(2));

            Triangle triangle(point1, point2, point3);
            double t_intersection;
            bool isPointInsideTriangle = calcIntersectionBetweenRayAndSingleTriangle(ray, triangle, t_intersection);

            if (isPointInsideTriangle == true)
            {
                isIntersecting = true;
                if (t_intersection < min_t || min_t < 0)
                {
                    min_t = t_intersection;
                }
            }
        }

        if (isIntersecting == true)
        {
            intersectionPoint = ray.point() + min_t * ray.unitVec();
        }

        return isIntersecting;

    }

    
    bool calcIntersectionBetweenRayAndMultiplesMeshes(std::vector<Mesh> meshes, cv::Vec3d& intersectionPoint, double& min_t, size_t& meshIntersectionIndex) {
        min_t = -1;
        bool isIntersecting = false;
        for (size_t i=0 ; i<meshes.size() ; i++) {
            Mesh currentMesh = meshes[i];
            double min_t_for_current_mesh;
            cv::Vec3d intersectionPointForCurrentMesh;
            bool isIntersectingCurrentMesh = calcIntersectionBetweenRayAndSingleMesh(currentMesh, intersectionPointForCurrentMesh, min_t_for_current_mesh);
            if (isIntersectingCurrentMesh == true)
            {
                isIntersecting = true;
                if (min_t_for_current_mesh < min_t || min_t < 0)
                {
                    min_t = min_t_for_current_mesh;
                    intersectionPoint = intersectionPointForCurrentMesh;
                    meshIntersectionIndex = i; 
                }
            }
        }
        return isIntersecting;
    }

    bool calcIntersectionBetweenRayAndSingleMesh(Mesh mesh, cv::Vec3d& intersectionPoint, double& min_t) {
        MathUseful mathUseful;
        cv::Mat rotation = mathUseful.calcRotation(ray.unitVec());
        Ray rayParallelToZ = ray.calcRotatedRay(rotation);
        cv::Mat1d rotatedVertices = mesh.calcRotatedVertices(rotation);
        cv::Mat1i trianglesIndexes = mesh.getIndicesOpenCV();
        std::vector<int> trianglesIndexesThatMightIntersectRay = calcTrianglesThatMightIntersectRay(rotatedVertices, rayParallelToZ, trianglesIndexes);


        bool isIntersecting = calcIntersectionBetweenRayAndMultipleTriangles(ray, mesh, trianglesIndexesThatMightIntersectRay, intersectionPoint, min_t);

        return isIntersecting;

    }
};