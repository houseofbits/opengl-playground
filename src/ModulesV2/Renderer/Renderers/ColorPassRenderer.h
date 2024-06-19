#pragma once

#include "../../../Renderer/Camera/Camera.h"
#include "../../../Renderer/Model/Cube.h"
#include "../../../Renderer/Shader/Shader.h"
#include "../Components/StaticMeshComponent.h"
#include "ColorPassFrameData.h"
class RendererSystem;

class ColorPassRenderer {
public:
    explicit ColorPassRenderer(RendererSystem*);

    RendererSystem* m_RenderSystem;

    void initialize();
    void beginRender(Camera&);
    void render(ColorPassFrameData&);

    Camera m_Camera;
    Shader m_Shader;
};
