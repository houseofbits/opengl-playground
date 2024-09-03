#pragma once

#include "../../../Core/API.h"
#include "../Helpers/ExcludeEntityIdFilterCallback.h"
#include "../Helpers/PhysicsActorUserData.h"
#include "../Helpers/RayCastResult.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class PhysicsResource : public Resource {
public:
    PhysicsResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;
    [[nodiscard]] float rayCast(glm::vec3 origin, glm::vec3 direction, float maxDistance, physx::PxQueryFilterCallback* callback) const;
    [[nodiscard]] float characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId);

    [[nodiscard]] bool characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId, RayCastResult& result);

    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxControllerManager* m_ControllerManager;

private:
    ExcludeEntityIdFilterCallback m_excludeEntityIdFilter;
};