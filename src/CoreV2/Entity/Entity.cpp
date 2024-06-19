#include "Entity.h"
#include "EntityContext.h"

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
    for (const auto &component: m_Components) {
        if (!component->isReady()) {
            return false;
        }
    }

    return true;
}
