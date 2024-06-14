#pragma once
#include "../../../libs/tinygltf/json.hpp"

class EntityContext;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    typedef std::shared_ptr<Component> TComponentPtr;

    unsigned int m_EntityId{};
    std::string m_Name;

    virtual void serialize(nlohmann::json&) = 0;
    virtual void deserialize(const nlohmann::json&) = 0;
    virtual void registerWithSystems(EntityContext&) = 0;
};
