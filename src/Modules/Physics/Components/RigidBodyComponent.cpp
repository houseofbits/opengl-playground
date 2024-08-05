#include "RigidBodyComponent.h"
#include "../../EditorUI/Systems/EditorUISystem.h"
#include "../Systems/PhysicsSystem.h"

RigidBodyComponent::RigidBodyComponent() : Component(), m_pxRigidBody(nullptr), m_density(100), m_friction(0.5,0.5), m_restitution(0.5) {
}

void RigidBodyComponent::serialize(nlohmann::json &j) {
    j["density"] = m_density;
    j["restitution"] = m_restitution;
    j["friction"] = m_friction;
}

void RigidBodyComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    m_density = j.value("density", m_density);
    m_restitution = j.value("restitution", m_restitution);
    m_friction = j.value("friction", m_friction);
}

void RigidBodyComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<PhysicsSystem>(this);
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
}

void RigidBodyComponent::updateBodyParameters() {
    if (m_pxRigidBody != nullptr) {
        physx::PxRigidBodyExt::updateMassAndInertia(*m_pxRigidBody, m_density);

//        physx::PxU32 nShapes = m_pxRigidBody->getNbShapes();
//        auto** shapes = new physx::PxShape*[nShapes];
//
//        m_pxRigidBody->getShapes(shapes, nShapes);
//        while (nShapes--)
//        {
//            shapes[nShapes]->getNbMaterials();
//        }
//        delete[] shapes;
    }
}
