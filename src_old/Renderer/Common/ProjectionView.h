#pragma once
#include <glm/mat4x4.hpp>

//TODO WIP: Implement ProjectionView
//What are the commonalities of LightView, Camera, Decal, RTT
class ProjectionView {
public:
    ProjectionView();

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionViewMatrix;
};
