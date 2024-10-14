#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/LightsBufferResource.h"

class ShadowMapRenderSystem : public EntitySystem {
public:
    ShadowMapRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleEditorUIEvent(const EditorUIEvent &);

private:
    void renderGeometry(int lightIndex);

    void prepareShadowMapResources();

    ResourceManager *m_ResourceManager;
    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
};
