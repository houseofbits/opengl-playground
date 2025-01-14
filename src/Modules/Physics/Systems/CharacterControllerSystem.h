#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Resources/PhysicsResource.h"

class CharacterControllerSystem : public EntitySystem {
public:
    CharacterControllerSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEditorUIEvent(const EditorUIEvent *);

private:
    void updateCCTs();

    void resetToInitialTransform();

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
