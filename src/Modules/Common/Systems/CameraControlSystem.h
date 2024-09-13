#pragma once

#include "../../../Core/API.h"
#include "../../EditorUI/Events/EditorUIEvent.h"
#include "../Components/CameraComponent.h"

class CameraControlSystem : public EntitySystem {
public:
    CameraControlSystem();

    struct TBN {
        glm::vec3 view;
        glm::vec3 up;
        glm::vec3 right;
    };

    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleInputEvent(InputEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);
    Camera* findActiveCamera();
    static TBN calculateTBN(glm::vec3 viewDirection);

    bool m_isEnabled;
};
