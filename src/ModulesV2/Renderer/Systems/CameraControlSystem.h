#pragma once

#include "../../../CoreV2/API.h"
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

    void registerComponent(Component *comp) override;
    void unregisterComponent(Component *comp) override;
    void process() override;
    void initialize(ResourceManager*) override;
    void registerEventHandlers(EventManager *eventManager) override;
    bool handleWindowEvent(WindowEvent *event);
    bool handleInputEvent(InputEvent *event);
    bool handleEditorUIEvent(EditorUIEvent *event);
    Camera* findActiveCamera();
    static TBN calculateTBN(glm::vec3 viewDirection);

    std::map<Identity::Type, CameraComponent *> m_cameraComponents;
    bool m_isEnabled;
};
