#include "GridPlane.h"

void GridPlane::create(float width, float height, int subdivision)
{
    float halfW = width / 2;
    float halfH = height / 2;

    setColor(glm::vec4(1, 1, 0, 0.3));

    float step = height / subdivision;
    for (float h = -halfH; h <= halfH; h += step)
    {
        addVertex(glm::vec3(-halfW, 0, h));
        addVertex(glm::vec3(halfW, 0, h));
    }

    step = width / subdivision;
    for (float w = -halfW; w <= halfW; w += step)
    {
        addVertex(glm::vec3(w, 0, -halfH));
        addVertex(glm::vec3(w, 0, halfH));
    }

    build();
}

void GridPlane::draw()
{
    WireFrame::draw();
}