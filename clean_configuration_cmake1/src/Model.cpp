#include"Model.h"

Model::Model(std::string modelName, ModelParameters params) {
	this->modelName = modelName;
	this->params = params;
}

ModelParameters Model::getParams() {
	return this->params;
}

std::string Model::getModelName() {
	return this->modelName;
}

void Model::setParams(ModelParameters params) {
	this->params = params;
}

void Model::updateModelMatrixByUserParameters()
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::rotate(identityMatrix, glm::radians(params.angleZ), glm::vec3(0.0, 0.0, 1.0));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(params.angleY), glm::vec3(0.0, 1.0, 0.0));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(params.angleX), glm::vec3(1.0, 0.0, 0.0));
	this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(params.translateX, params.translateY, params.translateZ));
	this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(params.scaleX, params.scaleY, params.scaleZ));
	this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(params.scaleUniform, params.scaleUniform, params.scaleUniform));
}

void Model::sendTransformationToVertexShader(Shader shader, Camera* camera)
{
	shader.setMat4("model", this->modelMatrix);
	shader.setMat4("view", camera->getViewMatrix());
	shader.setMat4("projection", camera->getProjectionMatrix());
}

glm::mat4 Model::getModelMatrix() {
	return this->modelMatrix;
}