#pragma once

#include "../../../Core/API.h"
#include "../Helpers/Window.h"

class WindowResource : public Resource {
public:
    WindowResource();

    Resource::Status fetchData(ResourceManager &) override;

    Resource::Status build() override;

    void destroy() override;

    void create(EventManager &);

    void swapBuffers() const;

    glm::ivec2 getViewportSize() const;

    static void setRelativeMouseMode(bool isRelativeMouseModeOn);

    Window* m_Window;
};
