#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/PhysicsResource.h"

class PhysicsBodyProcessingSystem : public EntitySystem {
public:
    PhysicsBodyProcessingSystem();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void process(EventManager &) override;

    void handleEditorUIEvent(const EditorUIEvent *);

private:
    void resetToInitialTransform();

    void wakeUpAll();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
