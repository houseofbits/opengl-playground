#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Layers.h"

class SensorLayerFilter final : public JPH::ObjectLayerFilter {
public:
    SensorLayerFilter(bool excludeActions) {
        m_excludeActions = excludeActions;
    }

    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer layer) const override {
        if (layer == Layers::SENSOR) {
            return false;
        }
        if (m_excludeActions && layer == Layers::SENSOR_WITH_ACTIONS) {
            return false;
        }
        return true;
    }

    bool m_excludeActions = false;
};
