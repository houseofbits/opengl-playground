#pragma once

#include "../../../Core/API.h"
#include "../Helpers/PhysicsUserData.h"
#include "../Helpers/RayCastResult.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include "../Helpers/ObjectLayerPairFilterImpl.h"
#include "../Helpers/ObjectVsBroadPhaseLayerFilterImpl.h"
#include "../Helpers/BroadPhaseLayerMapper.h"
#include "../Helpers/ContactListenerImpl.h"

class PhysicsResource : public Resource {
public:
    PhysicsResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;
    void simulate();
    [[nodiscard]] float characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId);
//    [[nodiscard]] bool characterRayCast(glm::vec3 origin, glm::vec3 direction, Identity::Type characterEntityId, RayCastResult& result);
    void addContactPoint(Identity::Type entityId, glm::vec3 point);
    void clearEntityContacts();
    JPH::PhysicsSystem& getSystem();
    JPH::BodyInterface& getInterface();

    std::map<Identity::Type, std::vector<glm::vec3>> m_entityContacts;
private:
    JPH::PhysicsSystem m_PhysicsSystem;
    ObjectLayerPairFilterImpl m_objectPairFilter;
    ObjectVsBroadPhaseLayerFilterImpl m_broadPhaseFilter;
    BroadPhaseLayerMapper m_broadPhaseLayerMapper;
    ContactListenerImpl m_contactListener;
    JPH::JobSystemThreadPool* m_jobPool;
    JPH::TempAllocatorImpl* m_tempAllocator;
};
