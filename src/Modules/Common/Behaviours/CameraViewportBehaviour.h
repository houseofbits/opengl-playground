#pragma once

#include "../../../Core/API.h"

class CameraViewportBehaviour : public EntityBehaviour {
TYPE_DEFINITION(CameraViewportBehaviour);

public:
    CameraViewportBehaviour();

    void registerEventHandlers(EventManager &) override;

    void handleWindowEvent(const WindowEvent &);

    std::string getDescription() override;

    std::string getDisplayName() override {
        return "Camera viewport resize";
    }
};
