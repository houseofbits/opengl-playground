#pragma once

#include "RenderQueueItem.h"
#include <vector>

class Camera;

class RenderQueue {
public:
    RenderQueue() {
        items.reserve(8192);
    }

    void add(const RenderQueueItem &item) {
        items.push_back(item);
    }

    void clear() { items.clear(); }

    auto &get() {
        return items;
    }

    void render(Camera& camera);

private:
    std::vector<RenderQueueItem> items;
};
