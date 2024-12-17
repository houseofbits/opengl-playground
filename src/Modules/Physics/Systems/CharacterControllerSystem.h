#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../Resources/PhysicsResource.h"

class CharacterControllerSystem : public EntitySystem {
public:
    CharacterControllerSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEditorUIEvent(const EditorUIEvent &);

private:
    void updateCCTs() const;

    void resetToInitialTransform() const;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
    RelatedComponentRegistry<TransformComponent, PhysicsCharacterComponent>* m_registry;
};
