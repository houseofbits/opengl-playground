#pragma once
#include "../../../libs/tinygltf/json.hpp"
#include "../Reflection/Identity.h"
#include "../Reflection/TypedClass.h"
#include "../Resources/ResourceManager.h"

class EntityContext;

class Component : public BaseClass {
public:
    Component() = default;
    virtual ~Component() = default;

    typedef std::shared_ptr<Component> TComponentPtr;

    Identity m_EntityId{};
    Identity m_Id{};
    std::string m_Name;

    virtual void serialize(nlohmann::json&) = 0;
    virtual void deserialize(const nlohmann::json &, ResourceManager &resourceManager) = 0;
    virtual void registerWithSystems(EntityContext&) = 0;
    // Validation to ensure the component can be registered with systems
    // For example: check if all the resources have been properly loaded.
    virtual bool isReady() {
        return true;
    }
};
