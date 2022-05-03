
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp" //for glm::rotate, glm::translate, glm::scale, glm::lookAt

class Camera {
private:
    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    float _angle;
    float _nearPlane;
    float _farPlane;
    float _screenWidth;
    float _screenHeight;

    glm::mat4 _view;
    glm::mat4 _projection; 

public:
    Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float angle, float nearPlane, float farPlane, unsigned int screenWidth, unsigned int screenHeight) {
        this->_cameraPos    = cameraPos;
        this->_cameraFront  = cameraFront;
        this->_cameraUp     = cameraUp;
        this->_angle        = angle;
        this->_nearPlane = nearPlane;
        this->_farPlane = farPlane;
        this->_screenWidth  = screenWidth;
        this->_screenHeight = screenHeight;

        this->_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        this->_projection = glm::perspective(glm::radians(angle), (float)screenWidth / (float)screenHeight, nearPlane, farPlane);
    }

    glm::mat4 view() const {
        return this->_view;
    }

    glm::mat4 projection() const {
        return this->_projection;
    }    

};