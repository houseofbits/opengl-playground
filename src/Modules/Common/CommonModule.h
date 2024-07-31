#pragma once

#include "../../Core/API.h"
#include "Components/TransformComponent.h"

class CommonModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>("TransformComponent");
    };

    void registerSystems(EntityContext &ctx) override {

    };
};
