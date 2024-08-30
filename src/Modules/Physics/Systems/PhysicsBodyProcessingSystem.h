#pragma once
#include "../../../Core/API.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Resources/PhysicsResource.h"

class PhysicsBodyProcessingSystem : public EntitySystem {
public:
    PhysicsBodyProcessingSystem();

    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    void process() override;
    bool handleEditorUIEvent(EditorUIEvent *event);

private:
    void createRigidBodies();
    void createStaticBodies();
    void updateRigidBodyTransforms();
    void updateStaticBodyTransforms();
    void resetToInitialTransform();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
