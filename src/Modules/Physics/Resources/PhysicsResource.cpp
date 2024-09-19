#include "PhysicsResource.h"
//#include "../Helpers/SceneFilterShader.h"


PhysicsResource::PhysicsResource() : Resource(),
                                     m_collisionConfiguration(nullptr),
                                     m_collisionDispatcher(nullptr),
                                     m_overlappingPairCache(nullptr),
                                     m_solver(nullptr),
                                     m_dynamicsWorld(nullptr) {
}

Resource::Status PhysicsResource::fetchData(ResourceManager &) {
    return Resource::STATUS_DATA_READY;
}

Resource::Status PhysicsResource::build() {

    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    return Resource::STATUS_READY;
}

void PhysicsResource::destroy() {
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_collisionDispatcher;
    delete m_collisionConfiguration;
}

float PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId) {

    return 0;
}

bool PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId, RayCastResult &result) {

    return false;
}

void PhysicsResource::addContactPoint(Identity::Type entityId, glm::vec3 point) {
    if (m_entityContacts.count(entityId) == 0) {
        m_entityContacts[entityId].reserve(16);
    }

    m_entityContacts[entityId].push_back(point);
}

void PhysicsResource::clearEntityContacts() {
    for (auto &pair: m_entityContacts) {
        pair.second.clear();
    }
}

void PhysicsResource::simulate() const {
    m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);
}
