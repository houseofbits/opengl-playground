#pragma once

#include "../../Core/API.h"
#include "Components/EntityBehaviourComponent.h"

class BehaviourModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<EntityBehaviourComponent>("behaviour");
    };

    void registerSystems(EntityContext &ctx) override {
    };
};
