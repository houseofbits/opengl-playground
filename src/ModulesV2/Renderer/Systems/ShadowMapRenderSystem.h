#pragma once

#include "../../../CoreV2/API.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/ShadowAtlasResource.h"

class ShadowMapRenderSystem : public EntitySystem {
public:
    ShadowMapRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);

    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<ShadowAtlasResource> m_ShadowAtlas;
};
