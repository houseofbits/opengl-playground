#pragma once

#include "AbstractComponentRegistry.h"
#include "ComponentsTuple.h"

template<class... Ts>
class RelatedComponentRegistry : public AbstractComponentRegistry {
public:
    ~RelatedComponentRegistry() override = default;

    typedef ComponentsTuple<Ts...> TupleT;
    typedef std::unordered_map<Identity::Type, TupleT> ContainerType;

    void registerComponents(Entity &entity) override {
        if (!TupleT::doesEntityContainAllComponents(entity)) {
            return;
        }

        m_container[entity.m_Id.id()].registerComponents(entity);
    }

    bool contains(Identity::Type id) override {
        return m_container.find(id) != m_container.end();
    }

    ContainerType& container() {
        return m_container;
    }

    ContainerType m_container{};
};