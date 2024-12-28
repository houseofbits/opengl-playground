#pragma once

#include "../../Core/API.h"
#include "Systems/ApplicationSystem.h"
#include "Systems/ApplicationEventsSystem.h"
#include "Systems/ApplicationPostProcessSystem.h"

class ApplicationModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<ApplicationSystem>(EntitySystemRegistry::MAIN_PROCESS, 0);
        ctx.registerEntitySystem<ApplicationEventsSystem>(EntitySystemRegistry::MAIN_PROCESS, 0);
        ctx.registerEntitySystem<ApplicationPostProcessSystem>(EntitySystemRegistry::MAIN_PROCESS, 10000);
    };

    void postRegister(EntityContext &ctx) override {
    }
};
