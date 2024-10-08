#include "EntityBehaviour.h"
#include "../Entity/Entity.h"

EntityBehaviour::EntityBehaviour() : BaseClass(), EventHandler(), m_Entity(nullptr), m_EventManager(nullptr) {

}

Identity::Type EntityBehaviour::getId() {
    return m_Entity->m_Id.id();
}

void EntityBehaviour::detachFromEntity() {
    m_Entity->removeBehaviour(this);
}
