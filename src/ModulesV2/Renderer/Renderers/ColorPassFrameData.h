#pragma once

#include "../../Common/Components/TransformComponent.h"
#include "../Buffers/EnvironmentProbeStorageBuffer.h"
#include "../Buffers/LightStorageBuffer.h"
#include "../Buffers/ProjectionSamplerStorageBuffer.h"
#include "../Components/StaticMeshComponent.h"
#include <unordered_map>

class ColorPassFrameData {
public:
    struct Element {
        StaticMeshComponent *mesh = nullptr;
        TransformComponent *transform = nullptr;
    };

    ColorPassFrameData() : m_Elements(),
                           m_LightBuffer(),
                           m_ProjectorBuffer(),
                           m_EnvironmentProbeBuffer() {
    }

    void initialize() {
        m_LightBuffer.initialize();
        m_ProjectorBuffer.initialize();
        m_EnvironmentProbeBuffer.initialize();
    }

    void add(unsigned int index, StaticMeshComponent *mesh, TransformComponent *transform) {
        m_Elements[index].mesh = mesh;
        m_Elements[index].transform = transform;
    };

    void clear() {
        m_Elements.clear();
    }

    std::unordered_map<unsigned int, Element> m_Elements;
    LightStorageBuffer m_LightBuffer;
    EnvironmentProbeStorageBuffer m_EnvironmentProbeBuffer;
    ProjectionSamplerStorageBuffer m_ProjectorBuffer;
};