#pragma once

#include "../../../Core/API.h"
#include "../Resources/WindowResource.h"

class ApplicationPostProcessSystem final : public EntitySystem {
public:
    ApplicationPostProcessSystem();

    void initialize(ResourceManager &, EventManager&) override;

    void process(EventManager &) override;

    void registerEventHandlers(EventManager &) override;

private:
    ResourceHandle<WindowResource> m_WindowResource;
};
