#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Components/CameraComponent.h"

class CameraControlSystem : public EntitySystem {
public:
    CameraControlSystem();

    void process() override;

    void initialize(ResourceManager *) override;

    void registerEventHandlers(EventManager *eventManager) override;

    bool handleWindowEvent(WindowEvent *event);

    Camera *findActiveCamera();
};
