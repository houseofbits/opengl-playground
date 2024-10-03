#include "EntityBehaviourComponent.h"
#include "../../Editor/Systems/EditorUISystem.h"
#include "../Systems/CameraMouseLookBehaviourSystem.h"
#include "../Systems/CameraMovementBehaviourSystem.h"

EntityBehaviourComponent::EntityBehaviourComponent() : Component(), m_behaviours() {

}

void EntityBehaviourComponent::serialize(nlohmann::json &j) {
    nlohmann::json values = nlohmann::json::array();
    for (const auto &behaviour: m_behaviours) {
        values.push_back(nlohmann::json{behaviour.first, behaviour.second});
    }

    j[BEHAVIOURS_KEY] = values;
}

void EntityBehaviourComponent::deserialize(const nlohmann::json &j, ResourceManager &resourceManager) {
    if (j.contains(BEHAVIOURS_KEY) && j[BEHAVIOURS_KEY].is_array()) {
        for (const auto &element: j[BEHAVIOURS_KEY]) {
            addBehaviour(element.at(0), element.at(1));
        }
    }
}

void EntityBehaviourComponent::registerWithSystems(EntityContext &ctx) {
    ctx.registerComponentWithEntitySystem<EditorUISystem>(this);
    ctx.registerComponentWithEntitySystem<CameraMouseLookBehaviourSystem>(this);
    ctx.registerComponentWithEntitySystem<CameraMovementBehaviourSystem>(this);
}

void EntityBehaviourComponent::addBehaviour(EntityBehaviourComponent::BehaviourType type) {
    if (!hasBehaviour(type)) {
        m_behaviours.emplace_back(true, type);
    }
}

void EntityBehaviourComponent::addBehaviour(bool isEnabled, EntityBehaviourComponent::BehaviourType type) {
    if (!hasBehaviour(type)) {
        m_behaviours.emplace_back(isEnabled, type);
    }
}

void EntityBehaviourComponent::removeBehaviour(EntityBehaviourComponent::BehaviourType type) {
    m_behaviours.remove_if([&type](std::pair<bool, BehaviourType> &value) {
        return value.second == type;
    });
}

bool EntityBehaviourComponent::hasBehaviour(BehaviourType type) {
    auto result = std::find_if(m_behaviours.begin(), m_behaviours.end(),
                               [&type](std::pair<bool, BehaviourType> &value) {
                                   return value.second == type;
                               });

    return result != m_behaviours.end();
}

bool EntityBehaviourComponent::hasActiveBehaviour(EntityBehaviourComponent::BehaviourType type) {
    auto result = std::find_if(m_behaviours.begin(), m_behaviours.end(),
                               [&type](std::pair<bool, BehaviourType> &value) {
                                   return value.second == type && value.first;
                               });

    return result != m_behaviours.end();
}