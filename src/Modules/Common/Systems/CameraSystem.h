#pragma once

#include "../../../Core/API.h"
#include "../../Common/Events/CameraActivationEvent.h"

class CameraSystem : public EntitySystem {
public:
    CameraSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &);

    void handleSystemEvent(const SystemEvent &);

private:
    glm::ivec2 m_viewportSize;
};
