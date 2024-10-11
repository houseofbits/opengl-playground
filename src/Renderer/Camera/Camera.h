#pragma once
#include "../../Core/Events/EventManager.h"
#include "../../Modules/Renderer/Resources/ShaderProgramResource.h"
#include "../Shader/Shader.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class WindowEvent;
class InputEvent;

class Camera {
public:
    glm::vec3 right;
    glm::vec3 up;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionViewMatrix;
    glm::vec3 direction;
    glm::vec3 position;
    float zFar;
    float fieldOfView;
    float aspectRatio;

public:
    Camera();

    void bind(Shader &shader);
    void bind(ShaderProgramResource &shader);

    Camera &setFieldOfView(float degrees);
    Camera &setViewportSize(unsigned int viewportWidth, unsigned int viewportHeight);
    Camera &setView(glm::vec3 direction, glm::vec3 up);
    Camera &setFromAngles(float horizontal, float vertical);
    Camera &setPosition(glm::vec3 position);
    Camera &setZFar(float);
    Camera &setViewMatrix(glm::mat4);
    Camera &setFromTransformMatrix(const glm::mat4&);

    glm::mat4 &getProjectionViewMatrix();
    glm::mat4 &getViewMatrix() { return viewMatrix; }
    glm::mat4 &getProjectionMatrix() { return projectionMatrix; }
    glm::vec3 &getViewDirection();
    glm::vec3 &getUpDirection();
    glm::vec3 &getPosition() { return position; };
    [[nodiscard]] glm::mat4 getTransformMatrix() const;

private:
    void calculateView();
    void calculateProjection();
};
