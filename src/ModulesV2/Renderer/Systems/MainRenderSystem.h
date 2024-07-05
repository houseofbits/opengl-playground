#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/SamplersIndexBufferResource.h"
#include "../Resources/ShaderProgramResource.h"

class MainRenderSystem : public EntitySystem {
public:
    MainRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);

private:
    Camera *findActiveCamera();
    TransformComponent *findTransform(Identity &entityId);

    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    ResourceHandle<SamplersIndexBufferResource> m_SamplersIndexBuffer;
};
