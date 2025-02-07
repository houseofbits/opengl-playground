#pragma once

#include "../../Core/API.h"
#include "Systems/EditorUISystem.h"
#include "Components/EditorCameraComponent.h"
// #include "Editors/EditorCameraComponentEdit.h"
#include "Systems/EditorCameraSystem.h"

class EditorUIModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<EditorCameraComponent>("editorCamera");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<EditorUISystem>(EntitySystemRegistry::MAIN_PROCESS, 10);
        ctx.registerEntitySystem<EditorCameraSystem>(EntitySystemRegistry::MAIN_PROCESS, 1);
    };

    void postRegister(EntityContext &ctx) override {
        // if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
        //     editorSystem->registerComponentEditor<EditorCameraComponent, EditorCameraComponentEdit>();
        // }
    }
};
