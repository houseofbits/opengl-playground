#include "ContactListenerImpl.h"
#include "PhysicsUserData.h"
#include "../Resources/PhysicsResource.h"

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
        const auto userData1 = reinterpret_cast<PhysicsUserData *>(inBody1.GetUserData());
        const auto userData2 = reinterpret_cast<PhysicsUserData *>(inBody2.GetUserData());
        m_physicsResource->m_sensorContacts.emplace_back(userData1->m_entityId, userData2->m_entityId);
    }

    if (inBody2.IsSensor()) {
        const auto userData1 = reinterpret_cast<PhysicsUserData *>(inBody1.GetUserData());
        const auto userData2 = reinterpret_cast<PhysicsUserData *>(inBody2.GetUserData());
        m_physicsResource->m_sensorContacts.emplace_back(userData2->m_entityId, userData1->m_entityId);
    }
}

void ContactListenerImpl::OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2,
                                             const JPH::ContactManifold &inManifold,
                                             JPH::ContactSettings &ioSettings) {
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) {
}
