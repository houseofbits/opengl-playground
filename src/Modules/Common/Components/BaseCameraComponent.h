#pragma once

#include "../../../Renderer/Camera/Camera.h"

class BaseCameraComponent {
public:
    BaseCameraComponent() : m_isActive(false), m_Camera() {}

    virtual ~BaseCameraComponent() = default;

    virtual void setActive(bool active) {
        m_isActive = active;
    }

    void setViewportSize(glm::ivec2 viewportSize) {
        m_Camera.setViewportSize(viewportSize.x, viewportSize.y);
    }

    Camera m_Camera;
    bool m_isActive;
};
