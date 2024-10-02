#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Camera/Camera.h"
#include "../../Common/Components/TransformComponent.h"
#include "../../Editor/Events/EditorUIEvent.h"
#include "../Resources/EnvironmentProbesBufferResource.h"
#include "../Resources/EnvironmentProbesCubeMapArrayResource.h"
#include "../Resources/LightsBufferResource.h"
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
    float calculateZFar(glm::vec3 position, glm::vec3 direction, glm::vec3 min, glm::vec3 max);

    ResourceHandle<ShaderProgramResource> m_ShaderProgram;
    ResourceHandle<LightsBufferResource> m_LightsBuffer;
    ResourceHandle<EnvironmentProbesCubeMapArrayResource> m_cubeMapArray;
    Camera m_Camera;
    bool m_isRenderEnabled;
};
