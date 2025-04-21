#include "StatefulJointBehaviour.h"
#include "../BasePhysicsJoint.h"

StatefulJointBehaviour::StatefulJointBehaviour() : m_isInitiallyOpen(false),
                                                   m_state(STATE_CLOSING),
                                                   m_velocity(5),
                                                   m_isFixedOnFinalState(false) {
}

void StatefulJointBehaviour::serialize(nlohmann::json &j) {
    j[INITIAL_STATE_KEY] = m_isInitiallyOpen;
    j[FIXED_KEY] = m_isFixedOnFinalState;
    j[VELOCITY_KEY] = m_velocity;
}

void StatefulJointBehaviour::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_isInitiallyOpen = j.value(INITIAL_STATE_KEY, m_isInitiallyOpen);
    m_isFixedOnFinalState = j.value(FIXED_KEY, m_isFixedOnFinalState);
    m_velocity = j.value(VELOCITY_KEY, m_velocity);
    m_state = m_isInitiallyOpen ? STATE_OPENING : STATE_CLOSING;
}

void StatefulJointBehaviour::processJoint(BasePhysicsJoint *joint) {
    constexpr float threshold = 0.01;
    const float position = joint->getUnitPosition();
    if (m_state == STATE_OPENING) {
        if (position > 1.0 - threshold) {
            joint->setMotorOff();
            m_state = STATE_OPEN;
            if (m_isFixedOnFinalState) {
                joint->lockInPlace();
            }
        } else {
            joint->setMotorVelocity(m_velocity);
            joint->activate();
        }
    }
    if (m_state == STATE_CLOSING) {
        if (position < threshold) {
            joint->setMotorOff();
            m_state = STATE_CLOSED;
            if (m_isFixedOnFinalState) {
                joint->lockInPlace();
            }
        } else {
            joint->setMotorVelocity(-m_velocity);
            joint->activate();
        }
    }
}

void StatefulJointBehaviour::toggleJointState(BasePhysicsJoint *joint) {
    if (m_state == STATE_CLOSED) {
        m_state = STATE_OPENING;
        if (m_isFixedOnFinalState) {
            joint->unLock();
        }
    } else if (m_state == STATE_OPEN) {
        m_state = STATE_CLOSING;
        if (m_isFixedOnFinalState) {
            joint->unLock();
        }
    }
}