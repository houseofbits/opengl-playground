#include "WireCube.h"

WireCube::WireCube() : WireModel() {
}

void WireCube::create(const glm::vec4 color, const float size) {
    setColor(color);

    float hs = size * 0.5f;

    // X oriented segments
    addSegment({-hs, -hs, -hs}, {hs, -hs, -hs});
    addSegment({-hs, hs, -hs}, {hs, hs, -hs});
    addSegment({-hs, -hs, hs}, {hs, -hs, hs});
    addSegment({-hs, hs, hs}, {hs, hs, hs});
    //Y oriented segments
    addSegment({-hs, hs, -hs}, {-hs, -hs, -hs});
    addSegment({hs, hs, -hs}, {hs, -hs, -hs});
    addSegment({-hs, hs, hs}, {-hs, -hs, hs});
    addSegment({hs, hs, hs}, {hs, -hs, hs});
    //Z oriented segments
    addSegment({-hs, -hs, hs}, {-hs, -hs, -hs});
    addSegment({-hs, hs, hs}, {-hs, hs, -hs});
    addSegment({hs, -hs, hs}, {hs, -hs, -hs});
    addSegment({hs, hs, hs}, {hs, hs, -hs});

    build();
}
