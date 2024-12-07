#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"

class OverlayRenderSystem : public EntitySystem {
public:
    OverlayRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);
};
