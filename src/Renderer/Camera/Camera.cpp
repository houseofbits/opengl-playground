#include "Camera.h"
#include "../../Modules/Renderer/Resources/ShaderProgramResource.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

Camera::Camera() : direction(0, 0, -1), up(0, 1, 0), right(-1, 0, 0),
                   position(0), projectionMatrix(0.0), viewMatrix(0.0), projectionViewMatrix(0.0),
                   fieldOfView(90.0f), aspectRatio(1.0), zFar(1000) {
}

glm::mat4 &Camera::getProjectionViewMatrix() {
    return projectionViewMatrix;
}

glm::vec3 &Camera::getViewDirection() {
    return direction;
}

glm::vec3 &Camera::getUpDirection() {
    return up;
}

Camera &Camera::setViewportSize(unsigned int viewportWidth, unsigned int viewportHeight)
{
    aspectRatio = (float) viewportWidth / (float) viewportHeight;

    calculateProjection();

    return *this;
}

Camera &Camera::setFieldOfView(float degrees)
{
    fieldOfView = degrees;

    calculateProjection();

    return *this;
}

Camera &Camera::setPosition(glm::vec3 pos) {
    position = pos;
    calculateView();

    return *this;
}

Camera &Camera::setZFar(float zfar) {
    zFar = zfar;
    calculateProjection();

    return *this;
}


Camera &Camera::setFromAngles(float horizontal, float vertical) {
    float verticalAngle = glm::radians(vertical);
    float horizontalAngle = glm::radians(horizontal);

    direction = glm::vec3(
            std::cos(verticalAngle) * std::sin(horizontalAngle),
            std::sin(verticalAngle),
            std::cos(verticalAngle) * std::cos(horizontalAngle));

    right = glm::vec3(
            std::sin(horizontalAngle - 3.14f / 2.0f),
            0,
            std::cos(horizontalAngle - 3.14f / 2.0f));

    right = glm::normalize(right);

    up = glm::cross(right, direction);
    up = glm::normalize(up);

    calculateView();

    return *this;
}

Camera &Camera::setView(glm::vec3 viewDirection, glm::vec3 upDirection) {

    direction = glm::normalize(viewDirection);
    right = glm::normalize(glm::cross(direction, upDirection));
    up = glm::normalize(glm::cross(right, direction));

    calculateView();

    return *this;
}

void Camera::bind(Shader &shader) {
    shader.setUniform("viewProjectionMatrix", getProjectionViewMatrix());
    shader.setUniform("viewPosition", getPosition());
}

void Camera::bind(ShaderProgramResource &shader) {
    shader.setUniform("viewProjectionMatrix", getProjectionViewMatrix());
    shader.setUniform("viewPosition", getPosition());
}

void Camera::calculateView() {
    viewMatrix = glm::lookAt(position, position + direction, up);
    projectionViewMatrix = projectionMatrix * viewMatrix;
}

void Camera::calculateProjection()
{
    projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, zFar);
    projectionViewMatrix = projectionMatrix * viewMatrix;
}

Camera &Camera::setViewMatrix(glm::mat4 m) {
    viewMatrix = m;

    projectionViewMatrix = projectionMatrix * viewMatrix;

    return *this;
}

Camera &Camera::setFromTransformMatrix(const glm::mat4& m) {
    setPosition(m[3]);
    setView(glm::vec3(m[2]), glm::vec3(m[1]));

    return *this;
}

glm::mat4 Camera::getTransformMatrix() const {
    glm::mat4 m = glm::mat4(1.0f);
    m[0] = glm::vec4(glm::normalize(glm::cross(direction, up)), 0.0f);
    m[1] = glm::vec4(glm::normalize(up), 0.0f);
    m[2] = glm::vec4(glm::normalize(direction), 0.0f);
    m[3] = glm::vec4(position, 1.0);

    return m;
}

