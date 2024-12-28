#pragma once

#include "../../../Core/API.h"
#include "../Resources/WindowResource.h"

class ApplicationSystem : public EntitySystem {
public:
    ApplicationSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEntityCreationEvent(const EntityCreationEvent &);

private:
    void setTime();

    ResourceManager *m_ResourceManager;
    ResourceHandle<WindowResource> m_WindowResource;
};
