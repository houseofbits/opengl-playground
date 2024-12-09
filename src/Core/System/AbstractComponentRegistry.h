#pragma once

class Entity;

class AbstractComponentRegistry {
public:
    virtual void registerComponents(Entity &entity) = 0;
};