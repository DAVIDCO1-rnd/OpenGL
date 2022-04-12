#pragma once
#include <vector>
#include <string>
#include <algorithm>

using std::string;
using std::vector;

class Object
{
protected:
	vector<float> verticesAndTextures;
	vector<unsigned int> indices;	
	vector<vector<float>> vertices;
	string name;
	float min_x;
	float max_x;
	float min_y;
	float max_y;
	float min_z;
	float max_z;


public:

	unsigned int VAO, VBO, EBO; //are public to let opengl change them

	virtual vector<float> getVerticesAndTextures() = 0; // pure virtual => abstract class
	virtual vector<unsigned int> getIndices() = 0; // pure virtual => abstract class
	virtual string getName() = 0; // pure virtual => abstract class
	virtual void updateVertices() = 0;

	float getMinX() { return min_x; }
	float getMaxX() { return max_x; }

	float getMinY() { return min_y; }
	float getMaxY() { return max_y; }

	float getMinZ() { return min_z; }
	float getMaxZ() { return max_z; }

	void updateEdges()
	{
		vector<float> xVals;
		vector<float> yVals;
		vector<float> zVals;
		for (int i = 0; i < vertices.size(); i++)
		{
			xVals.push_back(vertices[i][0]);
			yVals.push_back(vertices[i][1]);
			zVals.push_back(vertices[i][2]);
		}
		min_x = *std::min_element(xVals.begin(), xVals.end());
		max_x = *std::max_element(xVals.begin(), xVals.end());

		min_y = *std::min_element(yVals.begin(), yVals.end());
		max_y = *std::max_element(yVals.begin(), yVals.end());

		min_z = *std::min_element(zVals.begin(), zVals.end());
		max_z = *std::max_element(zVals.begin(), zVals.end());
	}
};