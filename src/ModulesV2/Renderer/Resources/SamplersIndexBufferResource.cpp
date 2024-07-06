#include "SamplersIndexBufferResource.h"
#include "../../../Helper/ShaderSourceLoader.h"

SamplersIndexBufferResource::SamplersIndexBufferResource() : Resource(), m_StorageBuffer() {
}

Resource::Status SamplersIndexBufferResource::build() {
    m_StorageBuffer.create(
            MAX_SAMPLERS,
            ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void SamplersIndexBufferResource::destroy() {
}

void SamplersIndexBufferResource::bind(ShaderProgramResource &shader) {
    m_StorageBuffer.bind();
}

int SamplersIndexBufferResource::getOrAppend(GLuint64 handleId) {

    //TODO: Solve duplicate entries
    m_StorageBuffer.append(handleId);
    m_StorageBuffer.updateAll();

    return int(m_StorageBuffer.currentSize) - 1;
}