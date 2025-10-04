#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/EnvironmentProbesCubeMapArrayResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/MaterialsBufferResource.h"
#include "../Resources/ShaderProgramResource.h"
#include "../../Common/Events/CameraActivationEvent.h"
#include "../../Common/Helpers/ActiveCameraHelper.h"
#include "../Components/StaticMeshComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/SkyComponent.h"
#include "../Resources/ShaderResource.h"

class DeferredRenderTargetResource;

class MainRenderSystem : public EntitySystem {
public:
    MainRenderSystem();

    void process(EventManager &) override;

    void initialize(ResourceManager &, EventManager &) override;

    void registerEventHandlers(EventManager &) override;

    void handleSystemEvent(const SystemEvent &);

    void handleEditorUIEvent(const EditorUIEvent &);

    void handleCameraActivationEvent(const CameraActivationEvent &event) {
        m_activeCameraHelper.m_activeCameraId = event.m_cameraComponentId;
    }

private:
    enum ShaderType {
        SHADER_SHADED = 0,
        SHADER_PROBES,
        SHADER_REFLECTION,
        SHADER_SKY,
        NUM_SHADERS
    };

    bool m_isEnabled;
    ShaderType m_shaderType;
    int m_viewportWidth;
    int m_viewportHeight;
    ResourceHandle<ShaderProgramResource> m_ShaderPrograms[ShaderType::NUM_SHADERS];
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<MaterialsBufferResource> m_MaterialsBuffer;
    ResourceHandle<EnvironmentProbesBufferResource> m_ProbesBuffer;
    ResourceHandle<EnvironmentProbesCubeMapArrayResource> m_ProbesCubeMapArray;
    ActiveCameraHelper m_activeCameraHelper;
    EntityRelatedComponentRegistry<TransformComponent, StaticMeshComponent> *m_meshComponentRegistry;
    EntityUniqueComponentRegistry<SkyComponent> *m_skyComponentRegistry;
    EntityRelatedComponentRegistry<TransformComponent, MeshComponent> *m_compositeMeshComponentRegistry;
    ResourceHandle<MaterialResource> m_defaultMaterial;
    ResourceHandle<DeferredRenderTargetResource> m_deferredRenderTarget;
    ResourceHandle<TextureResource> m_brdfLUTTexture;

    ResourceHandle<ShaderResource> m_lightingShader;
};
