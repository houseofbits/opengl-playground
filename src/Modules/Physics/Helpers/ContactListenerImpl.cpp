#include "ContactListenerImpl.h"
#include "PhysicsBodyUserData.h"
#include "../Resources/PhysicsResource.h"
#include "../Helpers/PhysicsShapeUserData.h"

using namespace JPH;

ContactListenerImpl::ContactListenerImpl() : ContactListener(), m_physicsResource(nullptr) {
}

JPH::ValidateResult ContactListenerImpl::OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2,
                                                           JPH::RVec3Arg inBaseOffset,
                                                           const JPH::CollideShapeResult &inCollisionResult) {
    // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListenerImpl::OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2,
                                         const JPH::ContactManifold &inManifold,
                                         JPH::ContactSettings &ioSettings) {
    if (inBody1.IsSensor()) {
        handleSensorContact(inBody2, inBody1, inManifold.mSubShapeID1);
        // const auto userData1 = reinterpret_cast<PhysicsBodyUserData *>(inBody1.GetUserData());
        // const auto userData2 = reinterpret_cast<PhysicsBodyUserData *>(inBody2.GetUserData());
        // m_physicsResource->m_sensorContacts.emplace_back(userData1->m_entityId, userData2->m_entityId);
    } else if (inBody2.IsSensor()) {
        handleSensorContact(inBody1, inBody2, inManifold.mSubShapeID2);
        // const auto userData1 = reinterpret_cast<PhysicsBodyUserData *>(inBody1.GetUserData());
        // const auto userData2 = reinterpret_cast<PhysicsBodyUserData *>(inBody2.GetUserData());
        // m_physicsResource->m_sensorContacts.emplace_back(userData2->m_entityId, userData1->m_entityId);
    }
}

void ContactListenerImpl::OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2,
                                             const JPH::ContactManifold &inManifold,
                                             JPH::ContactSettings &ioSettings) {
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) {
}

void ContactListenerImpl::handleSensorContact(const JPH::Body &collider, const JPH::Body &sensor,
                                              const JPH::SubShapeID &sensorSubShapeId) const {
    const auto sensorUserData = reinterpret_cast<PhysicsBodyUserData *>(sensor.GetUserData());
    const auto colliderUserData = reinterpret_cast<PhysicsBodyUserData *>(collider.GetUserData());

    const auto rawUserData = sensor.GetShape()->GetSubShapeUserData(sensorSubShapeId);
    const auto shapeUserData = reinterpret_cast<PhysicsShapeUserData *>(rawUserData);

    m_physicsResource->addSensorContact({
        colliderUserData->m_entityId,
        sensorUserData->m_entityId,
        shapeUserData->m_physicsShapeComponentId,
        shapeUserData->m_physicsShapeComponentName
    });
}
