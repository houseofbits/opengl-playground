#pragma once

#include "../../Core/API.h"
#include "Components/EntityBehaviourComponent.h"
#include "Systems/CameraMouseLookBehaviourSystem.h"
#include "Systems/CameraMovementBehaviourSystem.h"

class BehaviourModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<EntityBehaviourComponent>("behaviour");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<CameraMouseLookBehaviourSystem>();
        ctx.registerEntitySystem<CameraMovementBehaviourSystem>();
    };
};
