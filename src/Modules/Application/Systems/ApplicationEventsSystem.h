#pragma once

#include "../../../Core/API.h"
#include "../Resources/WindowResource.h"

class ApplicationEventsSystem : public EntitySystem {
public:
    ApplicationEventsSystem();

    void initialize(ResourceManager &, EventManager &) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    bool isRunning;

private:
    ResourceHandle<WindowResource> m_WindowResource;
};