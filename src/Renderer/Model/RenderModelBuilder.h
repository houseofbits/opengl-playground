#pragma once
#include "ModelConfigurationLoader.h"

class Model;

class RenderModelBuilder
{
public:
    RenderModelBuilder();

    static void buildFromModelConfiguration(const ModelConfigurationLoader::ModelConfiguration& modelConfigurationIn, Model& modelOut);

private:

};
