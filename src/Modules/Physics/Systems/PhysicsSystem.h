#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager*) override;
    void process() override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);
    bool handleInputEvent(InputEvent *event);
    void buildRigidBodies();
    void buildCCTs();
    void resetToInitialTransform();
    void processCCTInput(CameraComponent* camera, CharacterControllerComponent* cct, InputEvent *event);
    void updateRigidBodies();
    void updateCCTs();

    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxControllerManager* m_ControllerManager;
    bool m_isSimulationDisabled;
};
