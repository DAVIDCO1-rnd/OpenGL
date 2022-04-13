#pragma once
#include "Object.h"
#include <vector>
using std::vector;

class Mesh
{
private:
	string fileName;
	vector<unsigned int> indices;
	vector<float> vertices;

	bool loadOBJ(string path, vector<float>& out_vertices, vector<unsigned int>& out_faces)
	{
		//printf("Loading OBJ file %s...\n", path.c_str());

		FILE * file = fopen(path.c_str(), "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return false;
		}

		while (1) {
			char lineHeader[128];
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) {
				vector<float> vertex(3);
				fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
				out_vertices.push_back(vertex[0]);
				out_vertices.push_back(vertex[1]);
				out_vertices.push_back(vertex[2]);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				unsigned int index1, index2, index3;
				fscanf(file, "%d %d %d\n", &index1, &index2, &index3);
				out_faces.push_back(index1 - 0);
				out_faces.push_back(index2 - 0);
				out_faces.push_back(index3 - 0);
			}
		}
		//printf("Finised loading OBJ file %s...\n", path.c_str());
		return true;
	}

public:
	unsigned int VAO, VBO, EBO; //are public to let opengl change them

	void loadMesh(string fileName)
	{
		this->vertices.clear();
		this->indices.clear();
		loadOBJ(fileName, this->vertices, this->indices);
	}

	Mesh(string fileName)
	{
		this->fileName = fileName;
		loadMesh(fileName);
	}

	vector<unsigned int> getIndices()
	{
		return indices;
	}

	vector<float> getVertices()
	{
		return vertices;
	}


};

