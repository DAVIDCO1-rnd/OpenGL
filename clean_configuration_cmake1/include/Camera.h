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


class Camera
{
private:
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	std::string cameraName;

public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
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
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);

	void updateViewMatrixByUserParameters();
	void updateProjectionMatrixByUserParameters();

	glm::mat4 getViewMatrix() const {
		return this->viewMatrix;
	}

	void setViewMatrix(glm::mat4 viewMatrix) {
		this->viewMatrix = viewMatrix;
	}

	glm::mat4 getProjectionMatrix() const {
		return this->projectionMatrix;
	}

	void setProjectionMatrix(glm::mat4 projectionMatrix) {
		this->projectionMatrix = projectionMatrix;
	}
};
#endif