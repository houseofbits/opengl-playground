#include "../Include.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

void ShadowMapRenderer::init(unsigned int mapSize)
{
    mapSize = mapSize;

    shader.loadProgram("resources/shaders/lightDepth.vert", "resources/shaders/lightDepth.frag");

    glGenFramebuffers(1, &depthFBOId);

    // glGenTextures(1, &depthTextureId);
    // glBindTexture(GL_TEXTURE_2D, depthTextureId);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mapSize, mapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, mapSize, mapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBOId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "texid: " << depthTextureId << " texsize: " << mapSize << std::endl;
}

void ShadowMapRenderer::beginRender(Light *light)
{
    glm::mat4 projectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -100, 200);
    glm::mat4 viewMatrix = glm::lookAt(light->direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), light->position);
    glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

    shader.use();
    shader.setUniform("modelViewProjectionMatrix", modelViewProjectionMatrix);

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBOId);
    glViewport(0, 0, mapSize, mapSize);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureId, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapRenderer::endRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}