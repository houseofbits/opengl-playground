#include "ProjectionSamplerStorageBuffer.h"
#include "../../../Helper/ShaderSourceLoader.h"
#include "../../../ResourcesV2/ShaderProgramResource.h"

ProjectionSamplerStorageBuffer::ProjectionSamplerStorageBuffer() : ShaderStorageBuffer<GLuint64>() {
    uniformBindingIndex = ShaderSourceLoader::registerBindingIndex("ProjectionSamplerStorageBuffer");
}

void ProjectionSamplerStorageBuffer::initialize() {
    create(MAX_PROJECTORS, uniformBindingIndex);
}
int ProjectionSamplerStorageBuffer::getOrAppend(GLuint64 handleId) {

    //TODO: Solve duplicate entries
    append(handleId);
    updateAll();

    return int(currentSize) - 1;
}
