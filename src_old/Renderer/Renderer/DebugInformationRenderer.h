#pragma once
#include "../../Components/Scene.h"
#include "../../WireframeRenderer/LineMesh.h"
#include "../Camera/Camera.h"
#include "../Shader/Shader.h"
#include "BaseRenderer.h"

class DebugInformationRenderer : public BaseRenderer {
public:
    explicit DebugInformationRenderer(RenderManager *);

    void init();
    void renderLightCones(Scene &scene, Camera& camera);

    Shader lightWireframeShader;
    LineMesh lineMesh;
};
