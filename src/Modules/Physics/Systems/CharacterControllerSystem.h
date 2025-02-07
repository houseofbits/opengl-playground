#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../Components/PhysicsCharacterComponent.h"
#include "../Resources/PhysicsResource.h"

class CharacterControllerSystem : public EntitySystem {
public:
    CharacterControllerSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

private:
    void updateCCTs() const;

    void resetToInitialTransform() const;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
    EntityRelatedComponentRegistry<TransformComponent, PhysicsCharacterComponent>* m_registry;
};
