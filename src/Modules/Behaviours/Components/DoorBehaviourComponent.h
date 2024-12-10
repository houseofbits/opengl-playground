#pragma once

#include "../../../Core/API.h"

class DoorBehaviourComponent : public Component {
    TYPE_DEFINITION(DoorBehaviourComponent);

    inline static const std::string INITIAL_STATE_KEY = "initialState";
    inline static const std::string FIXED_KEY = "fixed";

public:
    enum DoorState {
        STATE_CLOSED,
        STATE_OPEN,
        STATE_CLOSING,
        STATE_OPENING,
    };

    DoorBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool m_isInitiallyOpen;
    DoorState m_state;
    bool m_isFixedOnFinalState;

    //m_openActivationTarget;
    //m_closeActivationTarget;
};
