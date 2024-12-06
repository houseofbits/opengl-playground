#pragma once

#include "../../Core/API.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Behaviours/CameraControlInputBehaviour.h"
#include "../Physics/Behaviours/CharacterControlBehaviour.h"
#include "Behaviours/CameraViewportBehaviour.h"
#include "Systems/TransformHierarchyProcessingSystem.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/CameraComponentEdit.h"
#include "Editors/TransformComponentEdit.h"
#include "Systems/CameraSystem.h"

class CommonModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>("transform");
        ctx.registerComponent<CameraComponent>("camera");
    };

    void registerBehaviours(EntityContext &ctx) override {
        ctx.registerBehaviour<CharacterControlBehaviour>();
        ctx.registerBehaviour<CameraViewportBehaviour>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<TransformHierarchyProcessingSystem>(10);
        ctx.registerEntitySystem<CameraSystem>(1);
    }

    void postRegister(EntityContext &ctx) override {
        auto editorSystem = ctx.getSystem<EditorUISystem>();
        if (editorSystem != nullptr) {
            editorSystem->registerComponentEditor<CameraComponent, CameraComponentEdit>();
            editorSystem->registerComponentEditor<TransformComponent, TransformComponentEdit>();
        }
    }
};
