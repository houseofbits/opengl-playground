#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Helper/Log.h"
#include "../Components/CameraComponent.h"
#include "../Components/StaticMeshComponent.h"
#include "../Renderers/ColorPassFrameData.h"
#include "../Renderers/ColorPassRenderer.h"

class RendererSystem : public EntitySystem {
public:
    RendererSystem();

    void registerComponent(Component *comp) override;
    void unregisterComponent(Component *comp) override;
    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);

    void updateFrameData();
    TransformComponent *findTransform(Identity &entityId);

    std::map<Identity::Type, StaticMeshComponent *> m_meshComponents;
    std::map<Identity::Type, LightComponent *> m_lightComponents;
    std::map<Identity::Type, TransformComponent *> m_transformComponents;
    std::map<Identity::Type, CameraComponent *> m_cameraComponents;

    ColorPassFrameData m_frame;
    ColorPassRenderer m_colorPassRenderer;

    int m_viewportWidth;
    int m_viewportHeight;
};
