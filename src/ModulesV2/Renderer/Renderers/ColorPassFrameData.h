#pragma once

#include "../../Common/Components/TransformComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/StaticMeshComponent.h"
#include <unordered_map>

class ColorPassFrameData {
public:
    struct Element {
        StaticMeshComponent *mesh = nullptr;
        TransformComponent *transform = nullptr;
        MaterialComponent *material = nullptr;
    };

    void add(unsigned int index, StaticMeshComponent *mesh, TransformComponent *transform, MaterialComponent *material) {
        m_Elements[index].mesh = mesh;
        m_Elements[index].transform = transform;
        m_Elements[index].material = material;
    };

    void clear() {
        m_Elements.clear();
    }

    std::unordered_map<unsigned int, Element> m_Elements;
};