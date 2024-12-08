#include "Entity.h"
#include "EntityContext.h"

Entity::Entity() : m_Id(),
                   m_Name(),
                   m_Status(CREATED),
                   m_Components()
//                   m_Systems()
{

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
