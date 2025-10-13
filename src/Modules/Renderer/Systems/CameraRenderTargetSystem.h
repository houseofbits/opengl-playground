#pragma once

#include "../../../Core/API.h"

class CameraComponent;
class RenderQueueResource;

class CameraRenderTargetSystem : public EntitySystem {
public:
    CameraRenderTargetSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    EntityUniqueComponentRegistry<CameraComponent> *m_cameraComponentRegistry;
    ResourceHandle<RenderQueueResource> m_mainRenderQueue;
};

