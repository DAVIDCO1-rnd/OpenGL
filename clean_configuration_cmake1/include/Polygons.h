#ifndef POLYGONS_CLASS_H
#define POLYGONS_CLASS_H

#include <vector>
#include <glad/glad.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace Polygons {

	struct Point2D {
		int X;
		int Y;
	};

	cv::Mat convertRgbToBinaryImageRgb(cv::Mat3b rgbImg);
	void GetContinousBoundaryPoints(unsigned short* InputImage, int Width_i, int Height_i, std::vector<Point2D>& BoundaryPoints);
	std::vector<std::vector<cv::Point>> calcPolygons(size_t width, size_t height);
	unsigned short* convertBinaryImageToLabelsImage(unsigned short* binaryImage, size_t width, size_t height);
}

#endif