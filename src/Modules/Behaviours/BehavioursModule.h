#pragma once

#include "../../Core/API.h"
#include "Components/MainCharacterBehaviourComponent.h"
#include "Components/GunBehaviourComponent.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/MainCharacterBehaviourComponentEdit.h"
#include "Editors/GunBehaviourComponentEdit.h"
#include "Systems/MainCharacterBehaviourSystem.h"
#include "Components/DoorBehaviourComponent.h"
#include "Editors/DoorBehaviourComponentEdit.h"
#include "Systems/DoorBehaviourSystem.h"
#include "Systems/GunBehaviourSystem.h"

class BehavioursModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<MainCharacterBehaviourComponent>("mainCharacterBehaviour");
        ctx.registerComponent<DoorBehaviourComponent>("doorBehaviour");
        ctx.registerComponent<GunBehaviourComponent>("gunBehaviour");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<MainCharacterBehaviourSystem>(1);
        ctx.registerEntitySystem<DoorBehaviourSystem>(2);
        ctx.registerEntitySystem<GunBehaviourSystem>(2);
    };

    void postRegister(EntityContext &ctx) override {
        auto editorSystem = ctx.getSystem<EditorUISystem>();
        if (editorSystem != nullptr) {
            editorSystem->registerComponentEditor<MainCharacterBehaviourComponent,
                MainCharacterBehaviourComponentEdit>();
            editorSystem->registerComponentEditor<DoorBehaviourComponent, DoorBehaviourComponentEdit>();
            editorSystem->registerComponentEditor<GunBehaviourComponent, GunBehaviourComponentEdit>();
        }
    }
};
