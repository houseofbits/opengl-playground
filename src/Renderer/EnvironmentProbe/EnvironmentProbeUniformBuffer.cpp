#include "EnvironmentProbeUniformBuffer.h"

EnvironmentProbeUniformBuffer::EnvironmentProbeUniformBuffer() : uniformBuffer(), maxActiveProbes(0), numActiveProbes(0) {
}

void EnvironmentProbeUniformBuffer::create(unsigned int maxProbes, unsigned int uniformBindingIndex) {
    maxActiveProbes = maxProbes;
    uniformBuffer.create(maxProbes, EnvironmentProbeUniformBufferType::DYNAMIC, uniformBindingIndex);
}

void EnvironmentProbeUniformBuffer::update(Scene &scene) {
    int index = 0;
    for (const auto &probe: scene.probes) {
        auto& probeUniform = uniformBuffer.get(index);
        probeUniform.position = probe->position;
        probeUniform.cubeMapTextureLayer = probe->cubeMapTextureLayer;
        probeUniform.boundingBoxMin = glm::vec4(probe->boundingBoxMin, 1);
        probeUniform.boundingBoxMax = glm::vec4(probe->boundingBoxMax, 1);

        std::cout<<"write uniform "<<index<<std::endl;

        index++;
    }

    numActiveProbes = index;
    uniformBuffer.update(0, numActiveProbes);
}

void EnvironmentProbeUniformBuffer::bind(Shader &shader) {
    uniformBuffer.bind();
    shader.setUniform("probes", uniformBuffer.getBufferId());
    shader.setUniform("numProbes", numActiveProbes);
}
