#pragma once

#include "../../Core/API.h"
#include "Components/DoorComponent.h"
#include "Systems/DoorUpdateSystem.h"

class WorldMechanicsModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<DoorComponent>("DoorComponent");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<DoorUpdateSystem>(1);
    };
};
