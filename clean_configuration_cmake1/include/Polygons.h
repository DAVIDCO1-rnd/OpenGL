#ifndef POLYGONS_CLASS_H
#define POLYGONS_CLASS_H

#include <vector>
#include <glad/glad.h>

namespace Polygons {

	struct Point2D {
		int X;
		int Y;
	};

	unsigned short* convertRgbToBinaryImage(unsigned char* rgbImage, size_t width, size_t height, unsigned short redCircleVal, unsigned short greenCircleVal, unsigned short blueCircleVal);
	void GetContinousBoundaryPoints(unsigned short* InputImage, int Width_i, int Height_i, std::vector<Point2D>& BoundaryPoints);
	std::vector<std::vector<Point2D>> calcPolygons(size_t width, size_t height);
	unsigned short* convertBinaryImageToLabelsImage(unsigned short* binaryImage, size_t width, size_t height);
}

#endif