#include "Entity.h"
#include "EntityContext.h"

Entity::Entity() : m_Id(),
                   m_Name(),
                   m_Status(CREATED),
                   m_Components() {
}

void Entity::addComponent(Component &component) {
    m_Components.push_back(Component::TComponentPtr(&component));
}

void Entity::setStatus(Status status) {
    m_Status = status;
}

bool Entity::isReadyToRegister() {
    return std::all_of(m_Components.begin(), m_Components.end(),
                       [](const auto &component) { return component->isReadyToRegister(); });
}

void Entity::removeComponent(const Component &c) {
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it) {
        if ((*it)->m_Id == c.m_Id) {
            m_Components.erase(it);

            return;
        }
    }
}

void Entity::initializeComponents(EntityContext &ctx) const {
    for (const auto &component: m_Components) {
        if (component->isDeserialized() && component->isReadyToInitialize(ctx)) {
            if (component->initialize(ctx)) {
                component->m_Status = Component::STATUS_INITIALIZED;
            } else {
                component->m_Status = Component::STATUS_INITIALIZATION_ERROR;
            }
        }
    }
}
