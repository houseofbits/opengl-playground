#pragma once

#include "../../../Core/API.h"
#include "../../Common/Components/CameraComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Components/CharacterControllerComponent.h"
#include "../Resources/PhysicsResource.h"

class CharacterControllerSystem : public EntitySystem {
public:
    CharacterControllerSystem();

    void initialize(ResourceManager*) override;
    void process() override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);
    bool handleInputEvent(InputEvent *event);

private:
    void createCCTs();
    void updateCCTs();
    void resetToInitialTransform();
    void processCCTInput(CameraComponent* camera, CharacterControllerComponent* cct, InputEvent *event);

    ResourceHandle<PhysicsResource> m_PhysicsResource;
    bool m_isSimulationDisabled;
};
