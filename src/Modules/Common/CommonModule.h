#pragma once

#include "../../Core/API.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/TransformHierarchyProcessingSystem.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/CameraComponentEdit.h"
#include "Editors/CameraComponentTransformEdit.h"
#include "Editors/TransformComponentEdit.h"
#include "Editors/TransformComponentTransformEdit.h"
#include "Systems/CameraSystem.h"
#include "Systems/EntityLinkingSystem.h"

class CommonModule final : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<TransformComponent>();
        ctx.registerComponent<CameraComponent>();
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<TransformHierarchyProcessingSystem>(EntitySystemRegistry::MAIN_PROCESS, 10);
        ctx.registerEntitySystem<CameraSystem>(EntitySystemRegistry::MAIN_PROCESS, 1);
        ctx.registerEntitySystem<EntityLinkingSystem>(EntitySystemRegistry::MAIN_PROCESS, 2);
    }

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            editorSystem->registerTransformComponentEditor<CameraComponent, CameraComponentTransformEdit>();
            editorSystem->registerTransformComponentEditor<TransformComponent, TransformComponentTransformEdit>();

            editorSystem->registerComponentEditor<TransformComponent>(processTransformComponentEditor);
            editorSystem->registerComponentEditor<CameraComponent>(processCameraComponentEditor);
        }
    }
};
