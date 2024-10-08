#include "Entity.h"
#include "EntityContext.h"

Entity::Entity() : m_Id(),
                   m_Name(),
                   m_Status(CREATED),
                   m_Components(),
                   m_Behaviours(),
                   m_Systems() {

}

void Entity::addComponent(Component &component) {
    m_Components.push_back(Component::TComponentPtr(&component));
}

void Entity::registerWithSystems(EntityContext &ctx) {
    for (const auto &component: m_Components) {
        component->registerWithSystems(ctx);
    }

    m_Status = ACTIVE;
}

void Entity::unregisterFromSystems(EntityContext &ctx) {
    for (const auto &component: m_Components) {
        ctx.unregisterComponentFromSystems(component.get());
    }
}

bool Entity::isReadyToRegister() {
    return std::all_of(m_Components.begin(), m_Components.end(),
                       [](const auto &component) { return component->isReady(); });
}

void Entity::removeComponent(Component &c) {
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it) {
        if ((*it)->m_Id == c.m_Id) {
            m_Components.erase(it);

            return;
        }
    }
}

void Entity::addBehaviour(EntityBehaviour &behaviour) {
    m_Behaviours.push_back(&behaviour);
}

void Entity::removeBehaviour(EntityBehaviour *behaviour) {
    for (auto it = m_Behaviours.begin(); it != m_Behaviours.end(); ++it) {
        if ((*it)->getTypeId() == behaviour->getTypeId()) {
            m_Behaviours.erase(it);

            return;
        }
    }
}

void Entity::registerBehaviourEventHandlers(EventManager &eventManager) {
    if (!m_Behaviours.empty()) {
        for (const auto &behaviour: m_Behaviours) {
            behaviour->registerEventHandlers(eventManager);
        }
    }
}

EntityBehaviour *Entity::findBehaviour(std::string typeName) {
    auto it = std::find_if(
            m_Behaviours.begin(),
            m_Behaviours.end(),
            [&](EntityBehaviour *p) {
                return p->getTypeName() == typeName;
            });
    if (it != m_Behaviours.end()) {
        return (*it);
    }

    return nullptr;
}
