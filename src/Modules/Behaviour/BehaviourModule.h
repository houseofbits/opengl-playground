#pragma once

#include "../../Core/API.h"
#include "Components/EntityBehaviourComponent.h"
#include "Systems/CameraInputBehaviourSystem.h"
#include "Systems/CharacterInputBehaviourSystem.h"

class BehaviourModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<EntityBehaviourComponent>("behaviour");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<CameraInputBehaviourSystem>();
        ctx.registerEntitySystem<CharacterInputBehaviourSystem>();
    };
};
