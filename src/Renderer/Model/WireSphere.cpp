#include "WireSphere.h"
#include <glm/gtc/matrix_transform.hpp>

WireSphere::WireSphere() {
}

void WireSphere::create(const glm::vec4 color, const float size) {
    setColor(color);

    constexpr unsigned int segments = 16;
    const float hs = size * 0.5f;

    createCircle(glm::mat4(1.0), hs, segments);

    createCircle(
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        hs,
        segments
    );

    createCircle(
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        hs,
        segments
    );

    build();
}
