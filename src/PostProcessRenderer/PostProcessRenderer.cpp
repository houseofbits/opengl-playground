#include "PostProcessRenderer.h"
#include "../Renderer/Texture2D.h"

void PostProcessRenderer::init(glm::vec4 rect, std::string vertexProgramFileName, std::string fragmentProgramFileName)
{
    shader.loadProgram(vertexProgramFileName, fragmentProgramFileName);

    std::vector<float> p = {
        rect.x,
        rect.y,
        0.,
        rect.z,
        rect.y,
        0.,
        rect.z,
        rect.w,
        0.,
        rect.x,
        rect.w,
        0.,
    };

    std::vector<float> tex = {
        0., 1.,
        1., 1.,
        1., 0.,
        0., 0.};

    std::vector<unsigned int> el = {
        0, 1, 2, 0, 2, 3};

    createBuffers(&el, &p, nullptr, &tex);
}

void PostProcessRenderer::draw()
{
    if (textureId)
    {
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setUniform("texture1", 0);
        VertexArray::draw();
    }
}