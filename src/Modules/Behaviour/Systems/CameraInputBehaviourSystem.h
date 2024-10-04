#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/CameraComponent.h"
#include "../Components/EntityBehaviourComponent.h"

class CameraInputBehaviourSystem : public EntitySystem {
public:
    CameraInputBehaviourSystem();

    void process() override;

    void initialize(ResourceManager *) override;

    void registerEventHandlers(EventManager *eventManager) override;

    bool handleInputEvent(InputEvent *event);
};
