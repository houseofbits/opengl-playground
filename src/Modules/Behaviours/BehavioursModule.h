#pragma once

#include "../../Core/API.h"
#include "Components/MainCharacterBehaviourComponent.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/MainCharacterBehaviourComponentEdit.h"
#include "Systems/MainCharacterBehaviourSystem.h"
#include "Components/DoorBehaviourComponent.h"
#include "Editors/DoorBehaviourComponentEdit.h"
#include "Systems/DoorBehaviourSystem.h"

class BehavioursModule : public EntityModule {
public:
    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<MainCharacterBehaviourComponent>("mainCharacterBehaviour");
        ctx.registerComponent<DoorBehaviourComponent>("doorBehaviour");
    };

    void registerSystems(EntityContext &ctx) override {
        ctx.registerEntitySystem<MainCharacterBehaviourSystem>(1);
        ctx.registerEntitySystem<DoorBehaviourSystem>(2);
    };

    void postRegister(EntityContext &ctx) override {
        auto editorSystem = ctx.getSystem<EditorUISystem>();
        if (editorSystem != nullptr) {
            editorSystem->registerComponentEditor<MainCharacterBehaviourComponent, MainCharacterBehaviourComponentEdit>();
            editorSystem->registerComponentEditor<DoorBehaviourComponent, DoorBehaviourComponentEdit>();
        }
    }
};
