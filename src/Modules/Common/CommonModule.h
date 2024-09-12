#pragma once

#include "../../Core/API.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/CameraControlSystem.h"

class CommonModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>("transform");
        ctx.registerComponent<CameraComponent>("camera");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<CameraControlSystem>(1);
    };
};
