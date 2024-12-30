#pragma once

#include "../../../Core/Reflection/Identity.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../Events/CameraActivationEvent.h"
#include "../Components/BaseCameraComponent.h"

class ActiveCameraHelper {
public:
    ActiveCameraHelper() : m_activeCameraId(0) {

    }

    Camera *find(EntityContext &ctx) {
        if (!m_activeCameraId) {
            auto e = ctx.findEntity([](Entity *e) {
                auto c = e->getComponent<BaseCameraComponent>();

                return c != nullptr && c->m_isActive;
            });

            if (e) {
                m_activeCameraId = e->m_Id.id();
            }
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

    Identity::Type m_activeCameraId;
};
