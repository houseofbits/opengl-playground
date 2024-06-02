#include "ModelComponent.h"

ModelComponent::ModelComponent() :
                                   model(),
                                   transform(1.0),
                                   diffuseTextureId(0),
                                   normalTextureId(0),
                                   specularTextureId(0)
{
}

void ModelComponent::load(std::string filename)
{
    model.createFromFile(filename);
}

void ModelComponent::render()
{
    model.draw();
}