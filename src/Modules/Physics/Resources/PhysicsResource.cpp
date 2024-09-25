#include "PhysicsResource.h"
#include "../Helpers/PhysicsTypeCast.h"
#include "../Helpers/PhysicsRayCastFilterCallback.h"
//#include "../Helpers/SceneFilterShader.h"

#include <BulletDynamics/MLCPSolvers/btDantzigSolver.h>
#include <BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h>
#include <BulletDynamics/MLCPSolvers/btMLCPSolver.h>

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

//    auto* mlcp = new btDantzigSolver();
//    auto* mlcp = new btSolveProjectedGaussSeidel();
//    m_solver = new btMLCPSolver(mlcp);
    m_solver = new btSequentialImpulseConstraintSolver();

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_overlappingPairCache, m_solver,
                                                  m_collisionConfiguration);

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

bool
PhysicsResource::characterRayCast(glm::vec3 p, glm::vec3 d, Identity::Type characterEntityId, RayCastResult &result) {
    btVector3 rayStart = PhysicsTypeCast::glmToBullet(p);
    btVector3 rayEnd = PhysicsTypeCast::glmToBullet(p + (d * 100.0f));

//    PhysicsRayCastFilterCallback rayCallback(rayStart, rayEnd);
//    rayCallback.m_excludedEntityId = characterEntityId;
    btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);

    m_dynamicsWorld->rayTest(rayStart, rayEnd, rayCallback);
    if (rayCallback.hasHit()) {
        const btCollisionObject* hitObject = rayCallback.m_collisionObject;
        auto *userData = (PhysicsUserData *) hitObject->getUserPointer();
        result.m_entityId = userData->m_entityId;
        result.m_touchPoint = PhysicsTypeCast::bulletToGlm(rayCallback.m_hitPointWorld);
        result.m_distance = rayCallback.m_closestHitFraction;

        return true;
    }

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

void PhysicsResource::simulate() {
    m_dynamicsWorld->stepSimulation(1.f / 60.f, 1000);

    clearEntityContacts();
    int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int j = 0; j < numManifolds; j++) {
        btPersistentManifold *contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(j);
        const btCollisionObject *obA = contactManifold->getBody0();
        const btCollisionObject *obB = contactManifold->getBody1();

        auto *userDataA = (PhysicsUserData *) obA->getUserPointer();
        auto *userDataB = (PhysicsUserData *) obB->getUserPointer();
        if (userDataA->m_contactReporting || userDataB->m_contactReporting) {
            int numContacts = contactManifold->getNumContacts();
            for (int k = 0; k < numContacts; k++) {
                btManifoldPoint &pt = contactManifold->getContactPoint(k);
//                btVector3 ptA = pt.getPositionWorldOnA();
//                btVector3 ptB = pt.getPositionWorldOnB();
//                btVector3 normalOnB = pt.m_normalWorldOnB;
//                btScalar appliedImpulse = pt.getAppliedImpulse();

                if (userDataA->m_contactReporting) {
                    addContactPoint(userDataA->m_entityId, PhysicsTypeCast::bulletToGlm(pt.getPositionWorldOnA()));
                }
                if (userDataB->m_contactReporting) {
                    addContactPoint(userDataB->m_entityId, PhysicsTypeCast::bulletToGlm(pt.getPositionWorldOnB()));
                }
            }
        }
    }
}
