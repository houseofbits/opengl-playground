#pragma once

#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "RenderTarget.h"
#include "../Helper/AtlasGraph.h"
#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include "../Helper/ShaderSourceLoader.h"

class ShadowAtlas
{
public:
    AtlasGraph atlasGraph;
    RenderTarget renderTarget;

    void init(unsigned int atlasSize)
    {
        atlasGraph.generate(atlasSize, atlasSize, 128, 128);
        renderTarget.create(RenderTarget::TARGET_COLOR, atlasSize, atlasSize);
    }

    void render(Renderer &renderer, Scene &scene)
    {
        renderTarget.beginRender();

        renderTarget.end();
    }
};