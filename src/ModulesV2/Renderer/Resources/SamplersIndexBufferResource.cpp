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
