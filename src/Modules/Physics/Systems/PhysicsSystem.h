#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/PhysicsComponent.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

private:
    void processCreation() const;

    void recreateAll() const;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    SameComponentRegistry<PhysicsComponent> *m_physicsComponentRegistry;
    bool m_isSimulationDisabled;
};
