#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"

class OverlayRenderSystem : public EntitySystem {
public:
    OverlayRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);
};
