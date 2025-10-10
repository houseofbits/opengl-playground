#include "RenderQueueBuilder.h"

RenderQueueBuilder::RenderQueueBuilder() = default;

void RenderQueueBuilder::build(RenderQueue &queue, const Model &model, RenderShader &shader,
                               const glm::mat4 &worldTransform) {
    for (const auto &mesh: model.m_meshNodes) {
        RenderQueueItem item;
        item.shader = &shader;
        item.vertexArray = model.m_vertexArrayObject;
        item.offset = mesh.offset;
        item.size = mesh.size;
        item.modelMatrix = mesh.modelMatrix * worldTransform;
        item.materialIndex = 1;

        queue.add(item);
    }
}
