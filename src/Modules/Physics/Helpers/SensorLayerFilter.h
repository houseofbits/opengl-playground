#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Layers.h"

class SensorLayerFilter final : public JPH::ObjectLayerFilter {
public:
    [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer layer) const override {
        if (layer == Layers::SENSOR) {
            return false;
        }
        return true;
    }
};
