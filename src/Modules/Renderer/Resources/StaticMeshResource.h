#pragma once

#include "../../../Core/API.h"
// #include "../../../Renderer/Model/VertexArray.h"

class StaticMeshResource : public Resource {
public:
    StaticMeshResource();

    Resource::Status build() override;
    void destroy() override {
        //TODO Unload the mesh resource
    }

    void render();

    // VertexArray m_VertexArray;
};
