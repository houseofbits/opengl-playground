#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Layers.h"

class BroadPhaseLayerMapper final : public JPH::BroadPhaseLayerInterface {
public:
    BroadPhaseLayerMapper() {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    }

    [[nodiscard]] unsigned int GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

    [[nodiscard]] virtual const char *GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const {
        switch ((JPH::BroadPhaseLayer::Type) inLayer) {
            case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::NON_MOVING:
                return "NON_MOVING";
            case (JPH::BroadPhaseLayer::Type) BroadPhaseLayers::MOVING:
                return "MOVING";
            default:
                JPH_ASSERT(false);
                return "INVALID";
        }
    }

private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS]{};
};
