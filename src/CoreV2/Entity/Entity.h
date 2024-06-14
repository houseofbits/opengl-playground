#pragma once

#include <string>
#include <list>
#include "../System/EntitySystem.h"

class Component;
class EntityContext;

class Entity
{
public:
    typedef std::shared_ptr<Entity> TEntityPtr;

    unsigned int m_Id;
    std::string m_Name;
    std::string m_TypeName;
    std::list<Component::TComponentPtr> m_Components;
    std::list<EntitySystem::TEntitySystemPtr> m_Systems;

    void addComponent(Component&);
    void registerWithSystems(EntityContext&);
    void unregisterFromSystems(EntityContext&);
};
