#include "PhysicsSliderJointComponent.h"
#include "../Helpers/PhysicsTypeCast.h"

PhysicsSliderJointComponent::PhysicsSliderJointComponent() : Component(),
                                                             BasePhysicsJoint(),
                                                             m_Joint(nullptr),
                                                             m_isLimitsEnabled(false),
                                                             m_axis(1, 0, 0),
                                                             m_limits(0.0),
                                                             m_PhysicsResource() {

}

void PhysicsSliderJointComponent::serialize(nlohmann::json &j) {
    j[ENTITY_KEY] = m_targetEntityName;
    j[ARE_LIMITS_ENABLED_KEY] = m_isLimitsEnabled;
    j[LIMITS_KEY] = m_limits;
    j[AXIS_KEY] = m_axis;
}

void PhysicsSliderJointComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_targetEntityName = j.value(ENTITY_KEY, m_targetEntityName);
    m_isLimitsEnabled = j.value(ARE_LIMITS_ENABLED_KEY, m_isLimitsEnabled);
    m_limits = j.value(LIMITS_KEY, m_limits);
    m_axis = j.value(AXIS_KEY, m_axis);

    resourceManager.request(m_PhysicsResource, "physics");
}

void PhysicsSliderJointComponent::registerWithSystems(EntityContext &ctx) {

}

bool PhysicsSliderJointComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void PhysicsSliderJointComponent::create(PhysicsBodyComponent &bodyA, PhysicsBodyComponent &bodyB) {
    if (!areAllowedToConnect(bodyA, bodyB)) {
        return;
    }

    JPH::SliderConstraintSettings settings;
    settings.mAutoDetectPoint = true;
    settings.SetSliderAxis(PhysicsTypeCast::glmToJPH(m_axis));
    if (m_isLimitsEnabled) {
        settings.mLimitsMin = m_limits.x;
        settings.mLimitsMax = m_limits.y;
    }

    m_Joint = (JPH::SliderConstraint *) settings.Create(*bodyA.m_physicsBody, *bodyB.m_physicsBody);

    m_PhysicsResource().getSystem().AddConstraint(m_Joint);
}

void PhysicsSliderJointComponent::release() {
    if (m_Joint != nullptr) {
        m_PhysicsResource().getSystem().RemoveConstraint(m_Joint);
        delete m_Joint;
        m_Joint = nullptr;
    }
}

bool PhysicsSliderJointComponent::isCreated() const {
    return m_Joint != nullptr;
}

void PhysicsSliderJointComponent::update() {

}
