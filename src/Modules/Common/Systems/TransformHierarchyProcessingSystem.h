#pragma once

#include "../../../Core/API.h"

class TransformHierarchyProcessingSystem : public EntitySystem {
public:
    TransformHierarchyProcessingSystem();

    void initialize(ResourceManager &) override;

    void process(EventManager &) override;
};
