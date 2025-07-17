#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Shader/ShaderStorageBuffer.h"

class MaterialsBufferResource : public Resource {
public:
    MaterialsBufferResource();

    struct alignas(16) MaterialStructure {
        glm::vec4 diffuseColor{1.0};
    };

    ShaderStorageBuffer<MaterialStructure> m_StorageBuffer;
};
