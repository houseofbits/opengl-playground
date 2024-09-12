#include "SimulationEventCallback.h"
#include "../Resources/PhysicsResource.h"

using namespace physx;

void SimulationEventCallback::onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs) {

    auto *data0 = static_cast<PhysicsActorUserData *>(pairHeader.actors[0]->userData);
    auto *data1 = static_cast<PhysicsActorUserData *>(pairHeader.actors[1]->userData);

    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair &cp = pairs[i];
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND || cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
            //            std::cout << "contact callback " << pairHeader.actors[0] << ", " << pairHeader.actors[1] << std::endl;

            PxContactPairPoint contactPoints[16];
            PxU32 contactCount = cp.extractContacts(contactPoints, 16);
            for (PxU32 j = 0; j < contactCount; j++) {
                m_physics->addContactPoint(data0->entityId, {contactPoints[j].position.x, contactPoints[j].position.y, contactPoints[j].position.z});
                m_physics->addContactPoint(data1->entityId, {contactPoints[j].position.x, contactPoints[j].position.y, contactPoints[j].position.z});

                //                                PxVec3 position = contactPoints[j].position;
                //                                PxVec3 normal = contactPoints[j].normal;
                //                                PxReal separation = contactPoints[j].separation;
                //
                //                                std::cout<<" CP:"<<position.x<<","<<position.y<<","<<position.z<<std::endl;
            }
        }
    }
}