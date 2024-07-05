#pragma once

#include "../../../Renderer/Camera/Camera.h"
#include "../../../Renderer/Model/Cube.h"
#include "../../../Renderer/Shader/Shader.h"
#include "../Components/StaticMeshComponent.h"
#include "../Resources/ShaderProgramResource.h"
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
