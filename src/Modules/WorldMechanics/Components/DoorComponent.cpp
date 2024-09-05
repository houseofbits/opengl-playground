#include "DoorComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/DoorUpdateSystem.h"

DoorComponent::DoorComponent() : Component(),
                                 m_movementType(TYPE_SLIDING),
                                 m_currentPosition(0.0),
                                 m_axis(0, 1, 0),
                                 m_rotationPivot(0, 0, 0),
                                 m_maxPosition(3.0),
                                 m_moveState(CLOSED) {
}

void DoorComponent::serialize(nlohmann::json &j) {
    j[TYPE_KEY] = (int) m_movementType;
    j[AXIS_KEY] = m_axis;
    j[PIVOT_KEY] = m_rotationPivot;
    j[MAX_POSITION_KEY] = m_maxPosition;
}

void DoorComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_movementType = j.value(TYPE_KEY, m_movementType);
    m_axis = j.value(AXIS_KEY, m_axis);
    m_rotationPivot = j.value(PIVOT_KEY, m_rotationPivot);
    m_maxPosition = j.value(MAX_POSITION_KEY, m_maxPosition);
}

void DoorComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<DoorUpdateSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void DoorComponent::activate() {
    if (m_moveState == CLOSED) {
        m_moveState = OPENING;
        m_currentPosition = 0.0;
    }
    if (m_moveState == OPEN) {
        m_moveState = CLOSING;
        m_currentPosition = 1.0;
    }
}

bool DoorComponent::isClosing() {
    return m_moveState == CLOSING;
}

bool DoorComponent::isOpening() {
    return m_moveState == OPENING;
}

void DoorComponent::setClosed() {
    m_moveState = CLOSED;
    m_currentPosition = 0.0;
}

void DoorComponent::setOpen() {
    m_moveState = OPEN;
    m_currentPosition = 1.0;
}
