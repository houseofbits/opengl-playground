#pragma once

#include "../../../Core/API.h"

class PhysicsTriggerShapeEvent : public BaseEvent {
    TYPE_DEFINITION(PhysicsTriggerShapeEvent);

private:
    PhysicsTriggerShapeEvent() : BaseEvent() {
    }

public:
    PhysicsTriggerShapeEvent(const Identity::Type sensorEntityId, const Identity::Type colliderEntityId) : BaseEvent(),
        m_sensorEntityId(sensorEntityId),
        m_colliderEntityId(colliderEntityId) {
    }

    Identity::Type m_sensorEntityId{0};
    Identity::Type m_colliderEntityId{0};
};
