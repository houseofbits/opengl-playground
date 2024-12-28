#pragma once

#include "../../../Core/API.h"

class PhysicsTriggerShapeComponent;
class TransformComponent;

class PhysicsTriggerShapeSystem final : public EntitySystem {
public:
    PhysicsTriggerShapeSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

private:
    RelatedComponentRegistry<TransformComponent, PhysicsTriggerShapeComponent>* m_registry;
};
