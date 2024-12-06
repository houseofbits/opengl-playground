#pragma once

#include "../../../Core/API.h"

class MainCharacterBehaviourComponent : public Component {
TYPE_DEFINITION(MainCharacterBehaviourComponent);

public:
    MainCharacterBehaviourComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &resourceManager) override;

    void registerWithSystems(EntityContext &ctx) override;

    bool m_isActive;


};
