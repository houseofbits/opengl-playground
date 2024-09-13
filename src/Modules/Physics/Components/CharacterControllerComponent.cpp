#include "CharacterControllerComponent.h"
#include "../../../Core/Helper/Types.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/CharacterControllerSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Helpers/FilterGroups.h"

using namespace physx;

CharacterControllerComponent::CharacterControllerComponent() : Component(),
                                                               m_height(1.75),
                                                               m_radius(0.25),
                                                               m_pxRigidActor(nullptr),
                                                               m_PhysicsResource() {
}

void CharacterControllerComponent::serialize(nlohmann::json &j) {
    j["height"] = m_height;
    j["radius"] = m_radius;
}

void CharacterControllerComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_height = j.value("height", m_height);
    m_radius = j.value("radius", m_radius);

    resourceManager.request(m_PhysicsResource, "physics");
}

void CharacterControllerComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<CharacterControllerSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void CharacterControllerComponent::create(TransformComponent &transform) {
    auto *rigidBody = m_PhysicsResource().m_pxPhysics->createRigidDynamic(Types::GLMtoPxTransform(transform.getModelMatrix()));
    rigidBody->setSleepThreshold(0.005f);

    m_pxRigidActor = rigidBody;

    m_pxRigidActor->userData = new PhysicsActorUserData(m_EntityId.id());

    PxCapsuleGeometry capsuleGeometry(m_radius, (m_height - m_radius * 2.0f) * 0.5f);
    physx::PxMaterial *material = m_PhysicsResource().m_pxPhysics->createMaterial(1.0, 1.0, 0.0);
    PxShape *capsuleShape = m_PhysicsResource().m_pxPhysics->createShape(capsuleGeometry, *material);

    PxFilterData filterData;
    filterData.word0 = FilterGroups::CHARACTER_CONTROLLER_CONTRACT_REPORTING;
    capsuleShape->setSimulationFilterData(filterData);

    PxTransform relativePose(PxVec3(0, m_height * 0.5f, 0), PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
    capsuleShape->setLocalPose(relativePose);
    m_pxRigidActor->attachShape(*capsuleShape);

    physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10);

    m_pxRigidActor->setMass(1);
    m_pxRigidActor->setLinearDamping(10.0f);
    m_pxRigidActor->setAngularDamping(100.0f);
    m_pxRigidActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
    m_pxRigidActor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
    m_pxRigidActor->setSleepThreshold(0);
}

bool CharacterControllerComponent::isReady() {
    return m_PhysicsResource.isReady();
}

void CharacterControllerComponent::update(TransformComponent &transform, bool isSimulationEnabled) const {
    if (isSimulationEnabled) {
        transform.setFromPxTransform(m_pxRigidActor->getGlobalPose());
    } else {
        m_pxRigidActor->setGlobalPose(Types::GLMtoPxTransform(transform.getModelMatrix()));
    }
}