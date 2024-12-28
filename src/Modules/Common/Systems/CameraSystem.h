#pragma once

#include "../../../Core/API.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../Components/CameraComponent.h"

class CameraSystem : public EntitySystem {
public:
    CameraSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    void registerEventHandlers(EventManager &) override;

    void handleInputEvent(const InputEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &);

    void handleSystemEvent(const SystemEvent &);

private:
    glm::ivec2 m_viewportSize;
    SingleComponentRegistry<CameraComponent>* m_registry;
};
