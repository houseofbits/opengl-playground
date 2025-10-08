#include "Shader.h"
#include "../../Core/Helper/Log.h"
#include "../Camera/Camera.h"
#include "Uniform/UniformValue.h"
#include <GL/glew.h>
#include <fstream>
#include <string>

Shader::~Shader() {
    glDeleteShader(programId);
}

void Shader::compileAndLink() {
    if (const auto result = compile()) {
        programId = result.value();
    }
}

void Shader::use() const {
    glUseProgram(programId);

    for (const auto &[location, value]: m_uniforms) {
        value->bind(location);
    }
}

void Shader::dispatchCompute(const unsigned int workGroupWidth, const unsigned int workGroupHeight) {
    glDispatchCompute(workGroupWidth, workGroupHeight, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Shader::setUniform(const char *name, const float x, const float y, const float z) {
    glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setUniform(const char *name, const glm::vec3 &v) {
    setUniform(name, v.x, v.y, v.z);
}

void Shader::setUniform(const char *name, const glm::vec4 &v) {
    glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const char *name, const glm::vec2 &v) {
    glUniform2f(getUniformLocation(name), v.x, v.y);
}

void Shader::setUniform(const char *name, const glm::mat4 &m) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, const glm::mat3 &m) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, float val) {
    glUniform1f(getUniformLocation(name), val);
}

void Shader::setUniform(const char *name, int val) {
    glUniform1i(getUniformLocation(name), val);
}

void Shader::setUniform(const char *name, const uint64_t &val) {
    glUniformHandleui64ARB(getUniformLocation(name), val);
}

void Shader::setUniform(const char *name, const unsigned int val) {
    glUniform1ui(getUniformLocation(name), val);
}

void Shader::setUniform(const char *name, bool val) {
    glUniform1i(getUniformLocation(name), val);
}

int Shader::getUniformLocation(std::string name) {
    if (programId == 0) {
        return 0;
    }

    if (const auto pos = uniformLocations.find(name); pos == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(programId, name.c_str());
    }

    return uniformLocations[name];
}

void Shader::addUniformValue(UniformValue *value, const std::string &name) {
    const auto location = getUniformLocation(name);

    m_uniforms[location] = value;
}

void Shader::use(Camera &camera) {
    use();

    setUniform("viewProjectionMatrix", camera.getProjectionViewMatrix());
    setUniform("viewPosition", camera.getPosition());
}
