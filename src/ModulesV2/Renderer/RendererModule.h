#pragma once

#include "../../CoreV2/API.h"
#include "Components/CameraComponent.h"
#include "Components/EnvironmentProbeComponent.h"
#include "Components/LightComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/CameraControlSystem.h"
#include "Systems/RendererSystem.h"

class RendererModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<StaticMeshComponent>("StaticMeshComponent");
        ctx.registerComponent<MaterialComponent>("MaterialComponent");
        ctx.registerComponent<LightComponent>("LightComponent");
        ctx.registerComponent<EnvironmentProbeComponent>("EnvironmentProbeComponent");
        ctx.registerComponent<CameraComponent>("CameraComponent");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<RendererSystem>();
        ctx.registerEntitySystem<CameraControlSystem>();
    };
};
