#pragma once

#include "../../Components/Scene.h"
#include "../Camera/Camera.h"
#include "../Texture/TextureCubeRenderTarget.h"
#include "BaseRenderer.h"

class EnvironmentProbeRenderer : public BaseRenderer {
public:
    const static int MAX_PROBES = 64;
    const static int PROBE_RESOLUTION = 128;
    static glm::vec3 cubeMapViewDirection[6];
    static glm::vec3 cubeMapUpDirection[6];

    explicit EnvironmentProbeRenderer(RenderManager *);

    void init(int maxProbes);
    void render(Scene& scene);

    TextureCubeRenderTarget cubeRenderTarget;
    Camera camera;
};
