#include "MainCharacterBehaviourComponent.h"
#include "../Systems/MainCharacterBehaviourSystem.h"


MainCharacterBehaviourComponent::MainCharacterBehaviourComponent() : Component(), m_isActive(true) {

}

void MainCharacterBehaviourComponent::serialize(nlohmann::json &j) {

}

void MainCharacterBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {

}

void MainCharacterBehaviourComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<MainCharacterBehaviourSystem>(this);
}
