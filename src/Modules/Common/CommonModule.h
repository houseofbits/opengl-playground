#pragma once

#include "../../Core/API.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Behaviours/CameraControlInputBehaviour.h"
#include "../Physics/Behaviours/CharacterControlBehaviour.h"
#include "Behaviours/CameraViewportBehaviour.h"

class CommonModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>("transform");
        ctx.registerComponent<CameraComponent>("camera");
    };

    void registerBehaviours(EntityContext & ctx) override {
        ctx.registerBehaviour<CharacterControlBehaviour>();
        ctx.registerBehaviour<CameraViewportBehaviour>();
    }
};
