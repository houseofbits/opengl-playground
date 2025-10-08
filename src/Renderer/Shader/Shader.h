#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "ShaderBuilder.h"

class UniformValue;
class Camera;

//@deprecated
class Shader : public ShaderBuilder {
public:
    ~Shader();

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

    int getUniformLocation(std::string name);

    [[nodiscard]] unsigned int getProgramId() const { return programId; }

    void addUniformValue(UniformValue *, const std::string &name);

    void use(Camera &camera);

private:
    unsigned int programId = 0;
    std::map<std::string, int> uniformLocations;
    std::unordered_map<int, UniformValue *> m_uniforms;
};
