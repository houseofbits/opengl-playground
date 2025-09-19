#pragma once

#include "../../../Core/API.h"

class CameraActivationEvent : public BaseEvent {
TYPE_DEFINITION(CameraActivationEvent);
public:
    CameraActivationEvent(Identity::Type id) : BaseEvent(), m_cameraComponentId(id) {

    }

    Identity::Type m_cameraComponentId;
};
