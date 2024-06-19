#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Helper/Log.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/StaticMeshComponent.h"
#include "../Renderers/ColorPassFrameData.h"
#include "../Renderers/ColorPassRenderer.h"

class RendererSystem : public EntitySystem {
public:
    RendererSystem();

    void registerComponent(Component *comp) override;

    void unregisterComponent(Component *comp) override;
    void process() override;
    void initialize() override;

    void createFrameData();
    TransformComponent* findTransform(Identity& entityId);
    MaterialComponent* findMaterial(Identity& entityId);

    std::map<Identity::Type, StaticMeshComponent *> m_meshComponents;
    std::map<Identity::Type, MaterialComponent *> m_materialComponents;
    std::map<Identity::Type, TransformComponent *> m_transformComponents;
    std::map<Identity::Type, CameraComponent *> m_cameraComponents;

    ColorPassFrameData m_frame;
    ColorPassRenderer m_colorPassRenderer;
};
