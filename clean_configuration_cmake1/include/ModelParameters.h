#ifndef MODEL_PARAMETERS_H
#define MODEL_PARAMETERS_H

#include <glad/glad.h>

class ModelParameters {
public:
	GLfloat angleX;
	GLfloat angleY;
	GLfloat angleZ;
	GLfloat scaleX;
	GLfloat scaleY;
	GLfloat scaleZ;
	GLfloat scaleUniform; //for uniform scaling (in all the axises)
	GLfloat translateX;
	GLfloat translateY;
	GLfloat translateZ;


	ModelParameters() {
		angleX = 0.0f;
		angleY = 0.0f;
		angleZ = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
		scaleZ = 1.0f;
		scaleUniform = 1.0f; //for uniform scaling (in all the axises)
		translateX = 0.0f;
		translateY = 0.0f;
		translateZ = 0.0f;
	}
};



#endif