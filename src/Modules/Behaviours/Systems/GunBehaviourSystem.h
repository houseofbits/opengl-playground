#pragma once

#include "../../../Core/API.h"
#include "../Components/GunBehaviourComponent.h"
#include "../../Common/Components/TransformComponent.h"

class GunBehaviourSystem : public EntitySystem {
public:
    GunBehaviourSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

private:
    RelatedComponentRegistry<TransformComponent, GunBehaviourComponent> *m_registry;
};
