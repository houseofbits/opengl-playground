#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "BaseShaderStorageBuffer.h"

class UniformValue;
class Camera;

class RenderShader final {
public:
    RenderShader() {
    }

    RenderShader(const unsigned int programId) : m_programId(programId) {
    }

    ~RenderShader();

    void use() const;

    void use(Camera &camera);

    void setUniform(const std::string &, float x, float y, float z);

    void setUniform(const std::string &, const glm::vec2 &v);

    void setUniform(const std::string &, const glm::vec3 &v);

    void setUniform(const std::string &, const glm::vec4 &v);

    void setUniform(const std::string &, const glm::mat4 &m);

    void setUniform(const std::string &, const glm::mat3 &m);

    void setUniform(const std::string &, const std::uint64_t &v);

    void setUniform(const std::string &, float val);

    void setUniform(const std::string &, int val);

    void setUniform(const std::string &, bool val);

    void setUniform(const std::string &, unsigned int val);

    void addStorageBuffer(BaseShaderStorageBuffer *buffer) {
        m_buffers.push_back(buffer);
    }

    void setProgramId(const unsigned int programId) {
        m_programId = programId;
    }

private:
    int getUniformLocation(std::string name);

    void setUniformValue(const std::string &, UniformValue *);

    unsigned int m_programId = 0;
    std::map<std::string, int> uniformLocations;
    std::unordered_map<int, UniformValue *> m_uniforms;
    std::vector<BaseShaderStorageBuffer *> m_buffers;
};
