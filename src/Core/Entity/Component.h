#pragma once
#include "../../../libs/tinygltf/json.hpp"
#include "../Reflection/Identity.h"
#include "../Reflection/TypedClass.h"
#include "../Resources/ResourceManager.h"

class EntityContext;

class Component : public BaseClass {
public:
    enum ComponentStatus {
        STATUS_CREATED = 0,
        STATUS_DESERIALIZED = 1,
        STATUS_INITIALIZED = 2,
        STATUS_REGISTERED = 3,
        //TODO removal statuses

        STATUS_INITIALIZATION_ERROR = 4,
    };

    Component() = default;

    virtual ~Component() {
    }

    typedef std::shared_ptr<Component> TComponentPtr;

    Identity m_EntityId{};
    Identity m_Id{};
    std::string m_Name;
    ComponentStatus m_Status{STATUS_CREATED};

    virtual void serialize(nlohmann::json &) = 0;

    virtual void deserialize(const nlohmann::json &, ResourceManager &resourceManager) = 0;

    virtual bool initialize(EntityContext& ctx) {
        return true;
    }

    // Validation to ensure the component can be initialized and has all the required resources ready
    // For example: check if all the resources have been properly loaded.
    [[nodiscard]] virtual bool isReadyToInitialize(EntityContext& ctx) const {
        return true;
    }

    template<class T>
    T *getInstance() {
        auto cp = dynamic_cast<T *>(this);
        if (cp) {
            return cp;
        }

        return nullptr;
    }

    //Called after the Component has been created and deserialized and it is ready to initialize (areResourcesReady)
    [[nodiscard]] bool isDeserialized() const {
        return m_Status == STATUS_DESERIALIZED;
    }

    [[nodiscard]] bool isReadyToRegister() const {
        return m_Status == STATUS_INITIALIZED;
    }

    [[nodiscard]] bool isInitialized() const {
        return m_Status == STATUS_INITIALIZED;
    }
};
