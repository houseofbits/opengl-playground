#pragma once

#include <string>
#include <map>
#include <optional>

class ShaderBuilder {
public:
    ShaderBuilder();

    void loadFromFile(const std::string &filepath);

    void loadFromString(unsigned int shaderGLType, const std::string &source);

    std::optional<unsigned int> compile();

private:
    static unsigned int loadShaderSource(unsigned int type, const std::string &source,
                                         const std::string &filename = "SOURCE");

    static unsigned int getShaderType(const std::string &filename);

    static bool checkCompileError(unsigned int shader, const std::string &name, const std::string &source);

    static void checkLinkingError(unsigned int shader);

    std::map<unsigned int, std::pair<std::string, std::string> > m_source;
};
