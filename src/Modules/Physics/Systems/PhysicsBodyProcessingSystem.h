#pragma once
#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/PhysicsResource.h"

class PhysicsBodyProcessingSystem : public EntitySystem {
public:
    PhysicsBodyProcessingSystem();

    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    void process() override;
    bool handleEditorUIEvent(EditorUIEvent *event);

private:
    void resetToInitialTransform();
    void wakeUpAll();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
