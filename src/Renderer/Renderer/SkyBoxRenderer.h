#pragma once

#include "../../Components/Scene.h"
#include "../Model/Cube.h"
#include "../Shader/Shader.h"
#include "../Texture/TextureCube.h"
#include "BaseRenderer.h"

class SkyBoxRenderer : public BaseRenderer {
private:
    Cube box;
    Shader shader;

public:
    SkyBoxRenderer(RenderManager *);

    void init();
    void render(Scene &scene);

    TextureCube texture;
};
