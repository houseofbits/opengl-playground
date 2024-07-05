#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/ShaderProgramResource.h"

class EnvironmentProbeRenderSystem : public EntitySystem {
public:
    EnvironmentProbeRenderSystem();

    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);


private:
    TransformComponent *findTransform(Identity &entityId);

    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
};
