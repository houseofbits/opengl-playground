#pragma once

#include "../../../Core/API.h"

class CameraMouseLookBehaviour {
public:
    CameraMouseLookBehaviour();

    void serialize(nlohmann::json &j);

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager);

    void process();

    void initialize(ResourceManager *);

    void registerEventHandlers(EventManager *eventManager);

    std::string getDescription();
};
