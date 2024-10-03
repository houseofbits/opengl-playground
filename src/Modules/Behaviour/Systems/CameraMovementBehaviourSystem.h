#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/CameraComponent.h"

class CameraMovementBehaviourSystem : public EntitySystem {
public:
    CameraMovementBehaviourSystem();

    void process() override;

    void initialize(ResourceManager *) override;

    void registerEventHandlers(EventManager *eventManager) override;

    bool handleInputEvent(InputEvent *event);
};
