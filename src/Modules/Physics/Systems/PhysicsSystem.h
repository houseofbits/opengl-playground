#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Resources/PhysicsResource.h"
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
    void buildBodies();
    void buildCCTs();
    void resetToInitialTransform();
    void processCCTInput(CameraComponent* camera, CharacterControllerComponent* cct, InputEvent *event);
    void updateBodies();
    void updateCCTs();

    bool m_isSimulationDisabled;

    ResourceHandle<PhysicsResource> m_PhysicsResource;
};
