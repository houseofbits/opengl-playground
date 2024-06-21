#pragma once

#include "../../../Renderer/Camera/Camera.h"
#include "../../../Renderer/Model/Cube.h"
#include "../../../Renderer/Shader/Shader.h"
#include "../../../ResourcesV2/ShaderProgramResource.h"
#include "../Components/StaticMeshComponent.h"
#include "ColorPassFrameData.h"
class RendererSystem;

class ColorPassRenderer {
public:
    explicit ColorPassRenderer(RendererSystem *);

    RendererSystem *m_RenderSystem;

    void initialize(ResourceManager *);
    void beginRender(Camera &);
    void render(ColorPassFrameData &);

    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
};
