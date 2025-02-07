#pragma once

#include "../../../Core/API.h"
#include "../../../Modules/Application/Events/SystemEvent.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/PhysicsBodyComponent.h"

class PhysicsBodyProcessingSystem : public EntitySystem {
public:
    PhysicsBodyProcessingSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void process(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

private:
    void resetToInitialTransform() const;

    bool m_isSimulationDisabled;
    EntityRelatedComponentRegistry<TransformComponent, PhysicsBodyComponent> *m_registry;
};
