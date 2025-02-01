#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../Resources/PhysicsResource.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

private:
    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
