#pragma once
#include "../../../Core/API.h"

class DoorComponent : public Component {
    TYPE_DEFINITION(DoorComponent);
public:
    inline static const std::string TYPE_KEY = "type";
    inline static const std::string AXIS_KEY = "axis";
    inline static const std::string PIVOT_KEY = "pivot";
    inline static const std::string MAX_POSITION_KEY = "maxPosition";

    enum MovementType {
        TYPE_SLIDING,
        TYPE_ROTATING,
    };

    DoorComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    void activate();
    bool isClosing();
    bool isOpening();
    void setClosed();
    void setOpen();

    MovementType m_movementType;
    glm::vec3 m_rotationPivot;
    glm::vec3 m_axis;   //Rotation axis or sliding direction
    float m_maxPosition;   //value for the OPEN position
    float m_currentPosition;

private:
    enum MovementState {
        CLOSED,
        OPEN,
        CLOSING,
        OPENING,
    };

    MovementState m_moveState;
};
