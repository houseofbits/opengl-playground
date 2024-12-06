#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/EnvironmentProbesCubeMapArrayResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/ShaderProgramResource.h"
#include "../../Common/Events/CameraActivationEvent.h"

class MainRenderSystem : public EntitySystem {
public:
    MainRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleWindowEvent(const WindowEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &);

private:
    enum ShaderType {
        SHADER_SHADED = 0,
        SHADER_PROBES,
        SHADER_REFLECTION,
        SHADER_SKY,
        NUM_SHADERS
    };

    Camera *findActiveCamera();

    bool m_isEnabled;
    ShaderType m_shaderType;
    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderPrograms[ShaderType::NUM_SHADERS];
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    ResourceHandle<EnvironmentProbesCubeMapArrayResource> m_ProbesCubeMapArray;
    Identity::Type m_activeCameraId;
};
