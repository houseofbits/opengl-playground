#include "RenderShader.h"
#include "../Camera/Camera.h"
#include "Uniform/UniformValue.h"
#include <GL/glew.h>
#include <fstream>
#include <string>
#include "Uniform/UniformValue1i.h"
#include "Uniform/UniformValue64i.h"
#include "Uniform/UniformValue1ui.h"
#include "Uniform/UniformValue1f.h"
#include "Uniform/UniformValue2f.h"
#include "Uniform/UniformValue3f.h"
#include "Uniform/UniformValue4f.h"
#include "Uniform/UniformValueMat3.h"
#include "Uniform/UniformValueMat4.h"

RenderShader::~RenderShader() {
    for (const auto &u: m_uniforms) {
        delete u.second;
    }

    m_uniforms.clear();

    glDeleteShader(m_programId);
}

void RenderShader::use() const {
    glUseProgram(m_programId);

    for (const auto &buffer: m_buffers) {
        buffer->bind();
    }

    for (const auto &[location, value]: m_uniforms) {
        value->bind(location);
    }
}

void RenderShader::use(Camera &camera) {
    glUseProgram(m_programId);

    addUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
    addUniform("viewPosition", camera.getPosition());

    for (const auto &buffer: m_buffers) {
        buffer->bind();
    }

    for (const auto &[location, value]: m_uniforms) {
        value->bind(location);
    }
}

void RenderShader::addUniform(const std::string &name, const float x, const float y, const float z) {
    setUniformValue(name, new UniformValue3f(x, y, z));
}

void RenderShader::addUniform(const std::string &name, const glm::vec3 &v) {
    setUniformValue(name, new UniformValue3f(v));
}

void RenderShader::addUniform(const std::string &name, const glm::vec4 &v) {
    setUniformValue(name, new UniformValue4f(v));
}

void RenderShader::addUniform(const std::string &name, const glm::vec2 &v) {
    setUniformValue(name, new UniformValue2f(v));
}

void RenderShader::addUniform(const std::string &name, const glm::mat4 &m) {
    setUniformValue(name, new UniformValueMat4(m));
}

void RenderShader::addUniform(const std::string &name, const glm::mat3 &m) {
    setUniformValue(name, new UniformValueMat3(m));
}

void RenderShader::addUniform(const std::string &name, const float val) {
    setUniformValue(name, new UniformValue1f(val));
}

void RenderShader::addUniform(const std::string &name, const int val) {
    setUniformValue(name, new UniformValue1i(val));
}

void RenderShader::addUniform(const std::string &name, const uint64_t &val) {
    setUniformValue(name, new UniformValue64i(val));
}

void RenderShader::addUniform(const std::string &name, const unsigned int val) {
    setUniformValue(name, new UniformValue1ui(val));
}

void RenderShader::addUniform(const std::string &name, const bool val) {
    setUniformValue(name, new UniformValue1i(val));
}

void RenderShader::setUniform(const std::string &name, const float x, const float y, const float z) {
    glUniform3f(getUniformLocation(name), x, y, z);
}

void RenderShader::setUniform(const std::string &name, const glm::vec3 &v) {
    setUniform(name, v.x, v.y, v.z);
}

void RenderShader::setUniform(const std::string &name, const glm::vec4 &v) {
    glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void RenderShader::setUniform(const std::string &name, const glm::vec2 &v) {
    glUniform2f(getUniformLocation(name), v.x, v.y);
}

void RenderShader::setUniform(const std::string &name, const glm::mat4 &m) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void RenderShader::setUniform(const std::string &name, const glm::mat3 &m) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void RenderShader::setUniform(const std::string &name, float val) {
    glUniform1f(getUniformLocation(name), val);
}

void RenderShader::setUniform(const std::string &name, int val) {
    glUniform1i(getUniformLocation(name), val);
}

void RenderShader::setUniform(const std::string &name, const uint64_t &val) {
    glUniformHandleui64ARB(getUniformLocation(name), val);
}

void RenderShader::setUniform(const std::string &name, const unsigned int val) {
    glUniform1ui(getUniformLocation(name), val);
}

void RenderShader::setUniform(const std::string &name, bool val) {
    glUniform1i(getUniformLocation(name), val);
}

int RenderShader::getUniformLocation(std::string name) {
    if (m_programId == 0) {
        return 0;
    }

    if (const auto pos = uniformLocations.find(name); pos == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(m_programId, name.c_str());
    }

    return uniformLocations[name];
}

void RenderShader::setUniformValue(const std::string &name, UniformValue *value) {
    const auto location = getUniformLocation(name);

    auto it = m_uniforms.find(location);
    if (it != m_uniforms.end()) {
        delete it->second;
    }

    m_uniforms[location] = value;
}
