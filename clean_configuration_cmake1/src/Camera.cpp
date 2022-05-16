#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position, float fovDeg, float nearPlane, float farPlane, std::string cameraName)
{
	this->cameraName = cameraName;
	Camera::width = width;
	Camera::height = height;
	this->position = position;
	this->fovDeg = fovDeg;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->updateMatrix();
}

void Camera::updateMatrix()
{
	// Initializes matrices since otherwise they will be the null matrix
	//this->viewMatrix = glm::mat4(1.0f);
	this->projectionMatrix = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	this->viewMatrix = glm::lookAt(position, position + orientation, up);
	glm::vec3 angles = this->mathUseful.getRotationAngles(this->viewMatrix);
	params.angleX = angles.x;
	params.angleY = angles.y;
	params.angleZ = angles.z;
	params.translateX = position.x;
	params.translateY = position.y;
	params.translateZ = position.z;
	// Adds perspective to the scene
	this->projectionMatrix = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	//cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(this->viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
}

void Camera::updateViewMatrixByUserParameters()
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	this->viewMatrix = glm::translate(identityMatrix, glm::vec3(-params.translateX, -params.translateY, -params.translateZ));
	this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(-params.angleX), glm::vec3(1.0, 0.0, 0.0));
	this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(-params.angleY), glm::vec3(0.0, 1.0, 0.0));
	this->viewMatrix = glm::rotate(this->viewMatrix, glm::radians(-params.angleZ), glm::vec3(0.0, 0.0, 1.0));	
}

void Camera::updateProjectionMatrixByUserParameters() {
	this->projectionMatrix = glm::perspective(glm::radians(fovDeg), (float)width / height, nearPlane, farPlane);

}

glm::mat4 Camera::getViewMatrix() const {
	return this->viewMatrix;
}

void Camera::setViewMatrix(glm::mat4 viewMatrix) {
	this->viewMatrix = viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
	return this->projectionMatrix;
}

void Camera::setProjectionMatrix(glm::mat4 projectionMatrix) {
	this->projectionMatrix = projectionMatrix;
}

std::string Camera::getCameraName() const {
	return this->cameraName;
}

CameraParameters Camera::getParams() const {
	return params;
}

void Camera::setParams(CameraParameters params) {
	this->params = params;
}

std::vector<std::vector<glm::vec3>> Camera::convert2dPixelsTo3dWorldCoordinates(std::vector<std::vector<cv::Point>> pixels) {
	cv::Mat1f depthImg(height, width);
	glReadPixels(0, 0, depthImg.cols, depthImg.rows, GL_DEPTH_COMPONENT, GL_FLOAT, depthImg.data);
	imshow("depth image", depthImg);
	cv::waitKey(0);

	std::vector<std::vector<glm::vec3>> worldCoordsPolygons;
	size_t numOfPolygons = pixels.size();
	for (size_t i = 0; i < numOfPolygons; i++) {
		std::vector<cv::Point> polygon = pixels[i];
		size_t numOfPointsInPolygon = polygon.size();
		std::vector<glm::vec3> currentPolygonWorldCoords;
		for (size_t j = 0; j < numOfPointsInPolygon; j++) {
			cv::Point pixel = polygon[j];
			int x = pixel.x;
			int y = pixel.y;

			float xNdc = ((2.0f * (float)x) / ((float)width - 1.0f)) - 1.0f;
			float yNdc = ((2.0f * ((float)height - 1.0f - y)) / ((float)height - 1.0f)) - 1.0f;
			float zNdc = depthImg.at<float>(x, y);

			glm::vec4 ndc = glm::vec4(xNdc, yNdc, zNdc, 1.0f);

			glm::mat projectionMatrixTranspose = glm::transpose(projectionMatrix);
			glm::mat viewMatrixTranspose = glm::transpose(viewMatrix);
			
			glm::mat4 vpMatrix = viewMatrixTranspose * projectionMatrixTranspose;
			glm::mat4 inverseVpMatrix = glm::inverse(vpMatrix);

			glm::vec4 temp = inverseVpMatrix * ndc;
			glm::vec3 worldCoords = glm::vec3(temp.x / temp.w, temp.y / temp.w, temp.z / temp.w);
			currentPolygonWorldCoords.push_back(worldCoords);			
		}
		worldCoordsPolygons.push_back(currentPolygonWorldCoords);
	}
	return worldCoordsPolygons;
}


void Camera::Inputs(GLFWwindow* window)
{
	 // Handles key inputs
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	 {
	 	position += speed * orientation;
	 }
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	 {
	 	position += speed * -glm::normalize(glm::cross(orientation, up));
	 }
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	 {
	 	position += speed * -orientation;
	 }
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	 {
	 	position += speed * glm::normalize(glm::cross(orientation, up));
	 }
	 if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	 {
	 	position += speed * up;
	 }
	 if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	 {
	 	position += speed * -up;
	 }
	 if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	 {
	 	speed = 0.4f;
	 }
	 else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	 {
	 	speed = 0.1f;
	 }


	 // Handles mouse inputs
	 if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	 {
	 	// Hides mouse cursor
	 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	 	// Prevents camera from jumping on the first click
	 	if (firstClick)
	 	{
	 		glfwSetCursorPos(window, (width / 2), (height / 2));
	 		firstClick = false;
	 	}

	 	// Stores the coordinates of the cursor
	 	double mouseX;
	 	double mouseY;
	 	// Fetches the coordinates of the cursor
	 	glfwGetCursorPos(window, &mouseX, &mouseY);

	 	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	 	// and then "transforms" them into degrees 
	 	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	 	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	 	// Calculates upcoming vertical change in the Orientation
	 	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	 	// Decides whether or not the next vertical Orientation is legal or not
	 	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	 	{
	 		orientation = newOrientation;
	 	}

	 	// Rotates the Orientation left and right
	 	orientation = glm::rotate(orientation, glm::radians(-rotY), up);

	 	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	 	glfwSetCursorPos(window, (width / 2), (height / 2));
	 }
	 else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	 {
	 	// Unhides cursor since camera is not looking around anymore
	 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	 	// Makes sure the next time the camera looks around it doesn't jump
	 	firstClick = true;
	 }
}