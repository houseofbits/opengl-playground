#include "ModelComponent.h"

ModelComponent::ModelComponent() : model(), transform(1.0)
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