#pragma once

#include "../../../Core/Reflection/Identity.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../Events/CameraActivationEvent.h"
#include "../Components/BaseCameraComponent.h"

class ActiveCameraHelper : public EventHandler {
public:
    ActiveCameraHelper() : m_activeCameraId(0) {

    }

    Camera *find(EntityContext &ctx) {
        if (!m_activeCameraId) {
            //TODO: Find active camera, or return first camera in the list
            return nullptr;
        }

        auto entity = ctx.getEntity(m_activeCameraId);

        if (!entity) {
            return nullptr;
        }

        auto camera = entity->getComponent<BaseCameraComponent>();
        if (camera != nullptr) {
            return &camera->m_Camera;
        }

        return nullptr;
    }

    void registerEventHandler(EventManager &eventManager) {
        eventManager.registerEventReceiver(this, &ActiveCameraHelper::handleCameraActivationEvent);
    }

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraId = event.m_cameraComponentId;
    }

    Identity::Type m_activeCameraId;
};
