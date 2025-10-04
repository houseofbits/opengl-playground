#pragma once

#include "../Reflection/Factory.h"
#include "Resource.h"

class ResourceFactory final : public Factory<Resource> {
public:
    ResourceFactory() = default;

    template<class T>
    void registerResourceType(const std::string name) {
        registerName<T>(name);
    }
};
