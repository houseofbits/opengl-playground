#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/EnvironmentProbesCubeMapArrayResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/ShaderProgramResource.h"

class MainRenderSystem : public EntitySystem {
public:
    MainRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);

private:
    enum ShaderType {
        SHADER_SHADED = 0,
        SHADER_PROBES,
        SHADER_REFLECTION,
        SHADER_SKY,
        NUM_SHADERS
    };
    Camera *findActiveCamera();

    ShaderType m_shaderType;
    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderPrograms[ShaderType::NUM_SHADERS];
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    ResourceHandle<EnvironmentProbesCubeMapArrayResource> m_ProbesCubeMapArray;
};
