#pragma once

#include "../../../Core/API.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem();

    void initialize(ResourceManager*) override;
    void process() override;
};
