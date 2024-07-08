#pragma once

#include "../../../CoreV2/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/EnvironmentProbesCubeMapArrayResource.h"
#include "../Resources/LightsBufferResource.h"
#include "../Resources/SamplersIndexBufferResource.h"
#include "../Resources/ShaderProgramResource.h"

class EnvironmentProbeRenderSystem : public EntitySystem {
public:
    static glm::vec3 m_cubeMapViewDirection[6];
    static glm::vec3 m_cubeMapUpDirection[6];

    EnvironmentProbeRenderSystem();

    void process() override;
    void initialize(ResourceManager *) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleEditorUIEvent(EditorUIEvent *event);

private:
    void bindGeometry();
    void renderGeometry();

    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<SamplersIndexBufferResource> m_SamplersIndexBuffer;
    ResourceHandle<EnvironmentProbesCubeMapArrayResource> m_cubeMapArray;
    Camera m_Camera;
    bool m_isRenderEnabled;
};
