#pragma once

#include "../../../Core/API.h"

class CameraControlInputBehaviour : public EntityBehaviour {
TYPE_DEFINITION(CameraControlInputBehaviour);

public:
    CameraControlInputBehaviour();

    void initialize(ResourceManager &) override;

    void registerEventHandlers(EventManager &) override;

    std::string getDescription() override;

    void handleInputEvent(const InputEvent &);

    std::string getDisplayName() override {
        return "Camera movement";
    }
};
