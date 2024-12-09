#pragma once

#include "../../../Core/API.h"
#include "../../../Renderer/Model/Cube.h"
#include "../Resources/CubeTextureResource.h"

class SkyComponent : public Component {
TYPE_DEFINITION(SkyComponent);

public:
    inline static const std::string CUBE_MAP_KEY = "cubeMap";

    SkyComponent();

    void serialize(nlohmann::json &j) override {
        j[CUBE_MAP_KEY] = m_cubeMap().m_Path;
    }

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override {
        std::string path = j.value(CUBE_MAP_KEY, m_cubeMap().m_Path);
        resourceManager.request(m_cubeMap, path);

        m_box.create(10.0);
    }

    std::string getListName(Entity *e) {
        return e->m_Name + " " + " SKY";
    }

    Cube m_box;
    ResourceHandle<CubeTextureResource> m_cubeMap;
};
