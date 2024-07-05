#include "LightsBufferResource.h"
#include "../../../Helper/ShaderSourceLoader.h"

LightsBufferResource::LightsBufferResource() : Resource(), m_StorageBuffer() {
}

Resource::Status LightsBufferResource::build() {
    m_StorageBuffer.create(
            MAX_SPOT_LIGHTS,
            ShaderSourceLoader::registerBindingIndex(m_Path));

    return STATUS_READY;
}

void LightsBufferResource::destroy() {
}
