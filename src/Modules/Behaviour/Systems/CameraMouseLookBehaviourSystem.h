#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/CameraComponent.h"

class CameraMouseLookBehaviourSystem : public EntitySystem  {
public:
    CameraMouseLookBehaviourSystem();

    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleInputEvent(InputEvent *event);

private:
    CameraComponent *findActiveCamera();
};
