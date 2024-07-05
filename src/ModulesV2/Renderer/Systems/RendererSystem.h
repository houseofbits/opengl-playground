#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Helper/Log.h"
#include "../Components/CameraComponent.h"
#include "../Components/EnvironmentProbeComponent.h"
#include "../Components/StaticMeshComponent.h"
#include "../Renderers/ColorPassFrameData.h"
#include "../Renderers/ColorPassRenderer.h"

class RendererSystem : public EntitySystem {
public:
    RendererSystem();

    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    void updateFrameData();
    TransformComponent *findTransform(Identity &entityId);
    Camera* findActiveCamera();

    ColorPassFrameData m_frame;
    ColorPassRenderer m_colorPassRenderer;

    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
};
