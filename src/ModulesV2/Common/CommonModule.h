#pragma once

#include "../../CoreV2/Module/EntityModule.h"
#include "Components/TransformComponent.h"

class CommonModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>("TransformComponent");
    };

    void registerSystems(EntityContext &ctx) override {

    };
};
