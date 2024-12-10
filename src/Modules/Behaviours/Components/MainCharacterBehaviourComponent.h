#pragma once

#include "../../../Core/API.h"

class MainCharacterBehaviourComponent : public Component {
TYPE_DEFINITION(MainCharacterBehaviourComponent);

public:
    MainCharacterBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    bool m_isActive;
    float m_mouseLookSpeed;
};
