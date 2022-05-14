#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <string>
#include"shaderClass.h"
#include "MathUseful.h"

class CameraParameters {
public:
	GLfloat angleX;
	GLfloat angleY;
	GLfloat angleZ;
	GLfloat translateX;
	GLfloat translateY;
	GLfloat translateZ;


	CameraParameters() {
		angleX = 0.0f;
		angleY = 0.0f;
		angleZ = 0.0f;
		translateX = 0.0f;
		translateY = 0.0f;
		translateZ = 0.0f;
	}
};


class Camera
{
private:
	
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float fovDeg;
	float nearPlane;
	float farPlane;
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	std::string cameraName;
	CameraParameters params;
	float angleDegX;
	float angleDegY;
	float angleDegZ;
	MathUseful mathUseful;

public:
	glm::vec3 position;
	// Stores the main vectors of the camera
	//glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position, float fov, float nearZ, float farZ, std::string cameraName);

	// Updates the camera matrix to the Vertex Shader
	void updateMatrix();
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);

	void updateViewMatrixByUserParameters();
	void updateProjectionMatrixByUserParameters();

	glm::mat4 getViewMatrix() const;

	void setViewMatrix(glm::mat4 viewMatrix);

	glm::mat4 getProjectionMatrix() const;

	void setProjectionMatrix(glm::mat4 projectionMatrix);

	std::string getCameraName() const;

	CameraParameters getParams() const;

	void setParams(CameraParameters params);

	std::vector<std::vector<glm::vec3>> convert2dPixelsTo3dWorldCoordinates(std::vector<std::vector<cv::Point>> pixels);
};
#endif