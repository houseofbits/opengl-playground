#pragma once

#include "../../CoreV2/API.h"
#include "Systems/EditorUISystem.h"

class EditorUIModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {

    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<EditorUISystem>();
    };
};
