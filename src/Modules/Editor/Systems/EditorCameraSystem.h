#pragma once

#include "../../../Core/API.h"
#include "../../Common/Events/CameraActivationEvent.h"

class EditorCameraSystem : public EntitySystem {
public:
    EditorCameraSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &);

    void handleWindowEvent(const WindowEvent &);

private:
    glm::ivec2 m_viewportSize;
};
