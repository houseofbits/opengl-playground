#pragma once

#include "../../../Core/API.h"

class PhysicsSensorEvent : public BaseEvent {
    TYPE_DEFINITION(PhysicsSensorEvent);

private:
    PhysicsSensorEvent() : BaseEvent() {
    }

public:
    PhysicsSensorEvent(const Identity::Type colliderEntityId,
                       const std::string &colliderName,
                       const Identity::Type sensorEntityId,
                       const std::string &sensorName,
                       const std::string &shapeName
    ) : BaseEvent(),
        m_colliderEntityId(colliderEntityId),
        m_colliderEntityName(colliderName),
        m_sensorEntityId(sensorEntityId),
        m_sensorEntityName(sensorName),
        m_sensorShapeName(shapeName) {
    }

    Identity::Type m_colliderEntityId{0};
    std::string m_colliderEntityName;
    Identity::Type m_sensorEntityId{0};
    std::string m_sensorEntityName;
    std::string m_sensorShapeName;
};
