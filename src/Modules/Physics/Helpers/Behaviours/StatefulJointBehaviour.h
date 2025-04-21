#pragma once

#include "../../../../Core/API.h"
#include <string>

class BasePhysicsJoint;

class StatefulJointBehaviour
{
    inline static const std::string INITIAL_STATE_KEY = "initialState";
    inline static const std::string FIXED_KEY = "fixed";
    inline static const std::string VELOCITY_KEY = "velocity";

public:
    enum DoorState {
        STATE_CLOSED,
        STATE_OPEN,
        STATE_CLOSING,
        STATE_OPENING,
    };

    StatefulJointBehaviour();

    void serialize(nlohmann::json &j);

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager);

    void processJoint(BasePhysicsJoint *joint);

    void toggleJointState(BasePhysicsJoint *joint);

    bool m_isInitiallyOpen;
    DoorState m_state;
    bool m_isFixedOnFinalState;
    float m_velocity;
};
