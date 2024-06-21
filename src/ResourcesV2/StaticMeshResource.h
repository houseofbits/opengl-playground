#pragma once

#include "../CoreV2/Resources/Resource.h"
#include "../Renderer/Model/VertexArray.h"

class StaticMeshResource : public Resource {
public:
    StaticMeshResource();

    void fetchData(ResourceManager&) override {
        setDataReadyStatus();
    }

    void build() override;

    void destroy() override {

    }

    void render();

    VertexArray m_VertexArray;
};
