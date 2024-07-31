#pragma once

#include "../../../Core/API.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Resources/LightsBufferResource.h"

class ShadowMapRenderSystem : public EntitySystem {
public:
    ShadowMapRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);
private:
    void renderGeometry(int lightIndex);
    void prepareShadowMapResources();

    ResourceManager* m_ResourceManager;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
};
