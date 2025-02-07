#pragma once

#include "../../../Core/API.h"
#include "../Components/GunBehaviourComponent.h"
#include "../../Common/Components/TransformComponent.h"

class GunBehaviourSystem : public EntitySystem {
public:
    GunBehaviourSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

private:
    EntityRelatedComponentRegistry<TransformComponent, GunBehaviourComponent> *m_registry;
};
