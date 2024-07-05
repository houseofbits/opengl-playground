#include "EnvironmentProbesBufferResource.h"
#include "../../../Helper/ShaderSourceLoader.h"

EnvironmentProbesBufferResource::EnvironmentProbesBufferResource() : Resource(), m_StorageBuffer() {
}

Resource::Status EnvironmentProbesBufferResource::build() {
    m_StorageBuffer.create(
            MAX_PROBES,
            ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void EnvironmentProbesBufferResource::destroy() {
}
