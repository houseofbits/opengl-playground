#pragma once

#include "../../../Core/API.h"
#include "../Helpers/PhysicsActorUserData.h"
#include "../Helpers/RayCastResult.h"
#include <bullet/btBulletDynamicsCommon.h>

class PhysicsResource : public Resource {
public:
    PhysicsResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;
    void simulate() const;
    [[nodiscard]] float characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId);
    [[nodiscard]] bool characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId, RayCastResult& result);
    void addContactPoint(Identity::Type entityId, glm::vec3 point);
    void clearEntityContacts();

    std::map<Identity::Type, std::vector<glm::vec3>> m_entityContacts;

    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_collisionDispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
};
