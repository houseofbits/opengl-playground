#pragma once

#include "../../../Core/API.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class CharacterControllerComponent : public Component {
    TYPE_DEFINITION(CharacterControllerComponent);
public:
    CharacterControllerComponent();

    void serialize(nlohmann::json &j) override;
    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;
    void registerWithSystems(EntityContext &ctx) override;
    glm::vec3 getPhysicsPosition() const;
    void setPhysicsPosition(glm::vec3);

    float m_height;
    float m_radius;
    physx::PxController* m_CCTController;
    glm::vec3 m_initialPosition;
};
