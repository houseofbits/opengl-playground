#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Helper/Log.h"
#include "../../Common/Components/TransformComponent.h"
#include "../Components/StaticMeshComponent.h"

class RendererSystem : public EntitySystem {
public:
    RendererSystem();

    void registerComponent(Component* comp) override {
        if (isOfType<StaticMeshComponent>(comp)) {
            Log::info("Register mesh component");
        }
        if (isOfType<TransformComponent>(comp)) {
            Log::info("Register transform component");
        }
    }

    void unregisterComponent(Component* comp) override {

    }

    void initialize() override;
};
