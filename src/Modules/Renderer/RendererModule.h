#pragma once

#include "../../Core/API.h"
#include "../Common/Components/CameraComponent.h"
#include "Components/EnvironmentProbeComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/EnvironmentProbeRenderSystem.h"
#include "Systems/MainRenderSystem.h"
#include "Systems/ShadowMapRenderSystem.h"
#include "Systems/StorageBufferUpdateSystem.h"
#include "Systems/OverlayRenderSystem.h"
#include "Systems/PhysicsDebugRenderSystem.h"

class RendererModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<StaticMeshComponent>("mesh");
        ctx.registerComponent<LightComponent>("light");
        ctx.registerComponent<EnvironmentProbeComponent>("probe");
        ctx.registerComponent<SkyComponent>("sky");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<StorageBufferUpdateSystem>(1);
        ctx.registerEntitySystem<ShadowMapRenderSystem>(2);
        ctx.registerEntitySystem<EnvironmentProbeRenderSystem>(3);
        ctx.registerEntitySystem<MainRenderSystem>(5);
        ctx.registerEntitySystem<PhysicsDebugRenderSystem>(6);
        ctx.registerEntitySystem<OverlayRenderSystem>(10);
    };
};
