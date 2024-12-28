#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Application/Events/SystemEvent.h"

class OverlayRenderSystem : public EntitySystem {
public:
    OverlayRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);
};
