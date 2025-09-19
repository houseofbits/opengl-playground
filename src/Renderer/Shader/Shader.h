#pragma once

#include <string>
#include <map>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader {
public:
    ~Shader();

    void loadFromFile(const std::string &filepath);

    void loadFromString(unsigned int shaderGLType, const std::string &source);

    void compileAndLink();

    void use() const;

    void dispatchCompute(unsigned int workGroupWidth = 16, unsigned int workGroupHeight = 16);

    void setUniform(const char *name, float x, float y, float z);

    void setUniform(const char *name, const glm::vec2 &v);

    void setUniform(const char *name, const glm::vec3 &v);

    void setUniform(const char *name, const glm::vec4 &v);

    void setUniform(const char *name, const glm::mat4 &m);

    void setUniform(const char *name, const glm::mat3 &m);

    void setUniform(const char *name, const std::uint64_t &v);

    void setUniform(const char *name, float val);

    void setUniform(const char *name, int val);

    void setUniform(const char *name, bool val);

    void setUniform(const char *name, unsigned int val);

    int getUniformLocation(const char *name);

    [[nodiscard]] unsigned int getProgramId() const { return programId; }

private:
    unsigned int programId = 0;
    std::map<std::string, int> uniformLocations;
    std::map<unsigned int, std::pair<std::string, std::string> > m_source;

    static unsigned int loadShaderSource(unsigned int type, const std::string &source,
                                         const std::string &filename = "SOURCE");

    static unsigned int getShaderType(const std::string &filename);

    static bool checkCompileError(unsigned int shader, const std::string &name, const std::string &source);

    static void checkLinkingError(unsigned int shader);
};
