#pragma once

#include "../../../Core/API.h"
#include "../../../Window.h"

class WindowResource : public Resource {
public:
    WindowResource();

    Resource::Status fetchData(ResourceManager &) override;

    Resource::Status build() override;

    void destroy() override;

    void create(EventManager &);

    [[nodiscard]] bool pollEvents() const;

    void swapBuffers() const;

    Window* m_Window;
};
