#pragma once

#include "AbstractComponentRegistry.h"
#include "ComponentsTuple.h"

template<class... Ts>
class RelatedComponentRegistry : public AbstractComponentRegistry {
public:
    typedef ComponentsTuple<Ts...> TupleT;
    typedef std::unordered_map<Identity::Type, TupleT> ContainerType;

    void registerComponents(Entity &entity) override {
        if (!TupleT::doesEntityContainAllComponents(entity)) {
            return;
        }

        m_container[entity.m_Id.id()].registerComponents(entity);
    }

    ContainerType& container() {
        return m_container;
    }

    ContainerType m_container{};
};