#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Resources/PhysicsResource.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager *) override;
    void process() override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
