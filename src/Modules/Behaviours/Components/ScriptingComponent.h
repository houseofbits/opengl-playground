#pragma once

#include "../../../Core/API.h"

class ScriptingComponent : public Component {
    TYPE_DEFINITION(ScriptingComponent);

    inline static const std::string SCRIPT_FILE_KEY = "scriptFile";
    inline static const std::string SCRIPT_SOURCE_KEY = "scriptSource";

public:
    ScriptingComponent();

    void serialize(nlohmann::json &j) override;

    void deserialize(const nlohmann::json &j, ResourceManager &) override;

    std::string m_scriptFile;
    std::string m_scriptSource;
};