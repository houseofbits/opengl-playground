#pragma once

#include "../../Core/API.h"
#include "Components/MainCharacterBehaviourComponent.h"
#include "Components/GunBehaviourComponent.h"
#include "../Editor/Systems/EditorUISystem.h"
#include "Editors/MainCharacterBehaviourComponentEdit.h"
#include "Editors/GunBehaviourComponentEdit.h"
#include "Editors/DoorBehaviourComponentEdit.h"
#include "Editors/ScriptingComponentEdit.h"
#include "Systems/MainCharacterBehaviourSystem.h"
#include "Components/DoorBehaviourComponent.h"
#include "Components/MotorBehaviourComponent.h"
#include "Components/ScriptingComponent.h"
#include "Systems/DoorBehaviourSystem.h"
#include "Systems/GunBehaviourSystem.h"
#include "Systems/MotorBehaviourSystem.h"
#include "Systems/ScriptingEventsSystem.h"
#include "Events/ScriptUpdateEvent.h"

class BehavioursModule final : public EntityModule {
public:
    static constexpr EntitySystemRegistry::ProcessType BEHAVIOUR_PROCESS = 1;
    static constexpr EntitySystemRegistry::ProcessType PHYSICS_PROCESS = 2;

    void registerComponents(EntityContext &ctx) override {
        ctx.registerComponent<MainCharacterBehaviourComponent>();
        ctx.registerComponent<DoorBehaviourComponent>();
        ctx.registerComponent<GunBehaviourComponent>();
        ctx.registerComponent<MotorBehaviourComponent>();
        ctx.registerComponent<ScriptingComponent>();
    }

    void registerSystems(EntityContext &ctx) override {
        ctx.entitySystemRegistry.createContinuousProcess(BEHAVIOUR_PROCESS, 32);

        ctx.registerEntitySystem<ScriptingEventsSystem>(BEHAVIOUR_PROCESS, 0);
        ctx.registerEntitySystem<DoorBehaviourSystem>(BEHAVIOUR_PROCESS, 2);
        ctx.registerEntitySystem<GunBehaviourSystem>(BEHAVIOUR_PROCESS, 3);
        ctx.registerEntitySystem<MotorBehaviourSystem>(BEHAVIOUR_PROCESS, 4);

        ctx.registerEntitySystem<MainCharacterBehaviourSystem>(PHYSICS_PROCESS, 5);
    }

    void postRegister(EntityContext &ctx) override {
        if (const auto editorSystem = ctx.getSystem<EditorUISystem>(); editorSystem != nullptr) {
            editorSystem->registerComponentEditor<MainCharacterBehaviourComponent>(
                processMainCharacterBehaviourComponentEditor);
            editorSystem->registerComponentEditor<DoorBehaviourComponent>(processDoorBehaviourComponentEditor);
            editorSystem->registerComponentEditor<GunBehaviourComponent>(processGunBehaviourComponentEditor);
            editorSystem->registerComponentEditor<ScriptingComponent>(processScriptingComponentEditor);
        }
    }

    void registerScriptableTypes(ScriptingManager &scriptingManager) override {
        scriptingManager.registerEventType<ScriptUpdateEvent>("ScriptUpdateEvent",
                                                              "timestamp", &ScriptUpdateEvent::m_timestamp
        );
    }
};
