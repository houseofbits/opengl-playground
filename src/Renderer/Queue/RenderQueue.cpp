#include "RenderQueue.h"
#include "../Camera/Camera.h"
#include "../Shader/RenderShader.h"

void RenderQueue::render(Camera &camera) {
    RenderShader *shader = nullptr;
    GLuint vertexArray = 0;
    for (const auto &item: items) {
        if (shader != item.shader) {
            item.shader->use(camera);
            shader = item.shader;
        }

        item.shader->setUniform("modelMatrix", item.modelMatrix);
        item.shader->setUniform("primaryMaterialIndex", item.materialIndex);

        if (vertexArray != item.vertexArray) {
            glBindVertexArray(item.vertexArray);
            vertexArray = item.vertexArray;
        }

        item.draw();
    }
}
