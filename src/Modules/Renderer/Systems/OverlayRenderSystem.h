#pragma once

#include "../../../Core/API.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../../Application/Events/SystemEvent.h"
#include "../Resources/ShaderProgramResource.h"
#include <GL/glew.h>
#include "../Resources/TextureResource.h"
#include "../Resources/RenderTargetResource.h"
#include "../../Application/Resources/WindowResource.h"

class DeferredRenderTargetResource;

class OverlayRenderSystem : public EntitySystem {
public:
    OverlayRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager&) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

private:
    void renderTexturedQuad(const TextureResource& texture);

    void renderTexturedQuad(std::uint64_t textureHandleId, glm::vec2 pos, glm::vec2 size);

    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    GLuint VAO{0};
    ResourceHandle<TextureResource> m_textureResource;
    ResourceHandle<TextureResource> m_crosshairTextureResource;
    ResourceHandle<DeferredRenderTargetResource> m_deferredRenderTarget;
    ResourceHandle<ShaderProgramResource> m_texturedQuadShaderProgram;

    ResourceHandle<WindowResource> m_WindowResource;

    ResourceHandle<RenderTargetResource> m_renderTarget;
};
