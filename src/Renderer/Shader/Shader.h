#pragma once

#include <string>
#include <map>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader {
private:
    unsigned int programId = 0;
    std::map<std::string, int> uniformLocations;

    unsigned int loadShader(const std::string &filename);

    unsigned int loadShaderSource(unsigned int type, const std::string &source, const std::string &filename = "SOURCE");

    unsigned int getShaderType(const std::string& filename);

    void checkCompileError(unsigned int shader, std::string name, std::string source);

    void checkLinkingError(unsigned int shader);

    void linkProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId, unsigned int geometryShaderId);

public:
    void loadProgram(const std::string &vertexProgramFileName, const std::string &fragmentProgramFileName,
                     const std::string &geometryProgramFileName = "");

    void loadProgramSource(const std::string &vertexProgram, const std::string &fragmentProgram);

    void use() const;

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

    unsigned int getProgramId() const { return programId; }
};
