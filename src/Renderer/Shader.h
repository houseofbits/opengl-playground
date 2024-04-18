#pragma once

#include <string>
#include <map>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Shader
{
private:
    unsigned int programId;
    std::map<std::string, int> uniformLocations;
    unsigned int loadShader(std::string filename);
    unsigned int getShaderType(std::string filename);
    void checkCompileError(unsigned int shader, std::string name);
    void checkLinkingError(unsigned int shader);

public:
    void loadProgram(std::string vertexProgramFileName, std::string fragmentProgramFileName, std::string geometryProgramFileName = "");
    void use();
    void setUniform(const char *name, float x, float y, float z);
    void setUniform(const char *name, const glm::vec2 &v);
    void setUniform(const char *name, const glm::vec3 &v);
    void setUniform(const char *name, const glm::vec4 &v);
    void setUniform(const char *name, const glm::mat4 &m);
    void setUniform(const char *name, const glm::mat3 &m);
    void setUniform(const char *name, float val);
    void setUniform(const char *name, int val);
    void setUniform(const char *name, bool val);
    void setUniform(const char *name, unsigned int val);
    int getUniformLocation(const char *name);
    unsigned int getProgramId() { return programId; }
};