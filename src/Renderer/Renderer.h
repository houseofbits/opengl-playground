#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include "Shader.h"
#include "Camera.h"

typedef struct alignas(16)
{
    glm::vec3 position;
    float falloff;

    glm::vec3 color;
    float intensity;

    glm::vec3 direction;
    unsigned int type;

    float distAttenMin;
    float distAttenMax;
    float beamAngle;
    float falloffAngle;
} ShaderLight;

class Light
{
public:
    enum Type
    {
        POINT = 0,
        DIRECT = 1,
    };

    Type type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float distAttenMin;
    float distAttenMax;
    float beamAngle;    // Half of the angle in radians
    float falloffAngle; // Half of the angle in radians
    Camera camera;

    Camera &getCamera()
    {
        glm::vec3 up = glm::cross(direction, glm::vec3(1, 0, 0));

        camera.setProjectionMatrix(glm::perspective<float>(glm::radians(60.0), 1.0, 0.01, distAttenMax));
        camera.setViewMatrix(glm::lookAt(position, position + direction, glm::vec3(1, 0, 0)));

        return camera;
    }
};

class Renderer
{
public:
    Renderer();

    static const unsigned int MAX_LIGHTS = 10;

    Camera *activeCamera;
    std::list<Light *> lights;
    ShaderLight shaderLights[Renderer::MAX_LIGHTS];
    unsigned int lightsUniformBufferId;
    unsigned int numActiveLights;

    Light *createPointLight(glm::vec3 pos, glm::vec3 color, float falloff, float intensity = 1.);
    Light *createDirectLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float beamAngle, float falloff, float intensity = 1.);
    void init(Camera *camera);
    void initLightsBuffer();
    void updateLights();
    void setShaderGlobalAttributes(Shader *shader);
};