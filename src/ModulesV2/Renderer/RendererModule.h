#pragma once

#include "../../CoreV2/API.h"
#include "Components/CameraComponent.h"
#include "Components/EnvironmentProbeComponent.h"
#include "Components/LightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/CameraControlSystem.h"
#include "Systems/EnvironmentProbeRenderSystem.h"
#include "Systems/MainRenderSystem.h"
#include "Systems/ShadowMapRenderSystem.h"
#include "Systems/StorageBufferUpdateSystem.h"

class RendererModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<StaticMeshComponent>("StaticMeshComponent");
        ctx.registerComponent<LightComponent>("LightComponent");
        ctx.registerComponent<EnvironmentProbeComponent>("EnvironmentProbeComponent");
        ctx.registerComponent<CameraComponent>("CameraComponent");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<CameraControlSystem>(1);
        ctx.registerEntitySystem<StorageBufferUpdateSystem>(1);
        ctx.registerEntitySystem<ShadowMapRenderSystem>(2);
        ctx.registerEntitySystem<EnvironmentProbeRenderSystem>(3);
        ctx.registerEntitySystem<MainRenderSystem>(5);
    };
};
