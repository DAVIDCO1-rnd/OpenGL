#ifndef POLYGONS_CLASS_H
#define POLYGONS_CLASS_H

#include <vector>
#include <glad/glad.h>

namespace Polygons {

	struct Point2D {
		int X;
		int Y;
	};

	unsigned char* convertRgbToBinaryImage(unsigned char* rgbImage, int width, int height, unsigned char redCircleVal, unsigned char greenCircleVal, unsigned char blueCircleVal);
	void GetContinousBoundaryPoints(unsigned char* InputImage, int Width_i, int Height_i, std::vector<Point2D>& BoundaryPoints);
	std::vector<Point2D> calcPolygons(int width, int height);
	unsigned char* convertBinaryImageToLabelsImage(unsigned char* binaryImage, int width, int height);
}

#endif