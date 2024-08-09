#include "ShaderProgramResource.h"
#include "../../../Core/Helper/StringUtils.h"
#include <GL/glew.h>
#include <sstream>
#include <vector>

ShaderProgramResource::ShaderProgramResource() : Resource(), m_ShaderHandles(), m_ProgramId(), m_uniformLocations() {
    m_ShaderHandles.reserve(3);
}

Resource::Status ShaderProgramResource::fetchData(ResourceManager& manager) {
    auto paths = StringUtils::getAlternatePaths(m_Path);
    if (paths.empty()) {
        Log::error("Resource path " + m_Path + " not valid for ShaderProgramResource");
        return STATUS_FETCH_ERROR;
    }

    for (const auto &path: paths) {
        addDependency(path);
        m_ShaderHandles.emplace_back();
        manager.request(m_ShaderHandles.back(), path);
    }

    return STATUS_DATA_READY;
}

Resource::Status ShaderProgramResource::build() {
    m_ProgramId = glCreateProgram();

    for (auto & shaderHandle : m_ShaderHandles) {
        glAttachShader(m_ProgramId, shaderHandle().m_Handle);
    }

    glLinkProgram(m_ProgramId);

    if (!checkLinkingError()) {
        return STATUS_BUILD_ERROR;
    }

    return STATUS_READY;
}

void ShaderProgramResource::destroy() {
    for (auto & shaderHandle : m_ShaderHandles) {
        shaderHandle.invalidate();
    }
    glDeleteProgram(m_ProgramId);
}

bool ShaderProgramResource::checkLinkingError() const
{
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ProgramId, 1024, nullptr, infoLog);
        std::cout << "LINKING FAILED " << infoLog << std::endl;

        return false;
    }

    return true;
}
void ShaderProgramResource::use()
{
    glUseProgram(m_ProgramId);
}

void ShaderProgramResource::setUniform(const char *name, float x, float y, float z)
{
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, x, y, z);
}

void ShaderProgramResource::setUniform(const char *name, const glm::vec3 &v)
{
    this->setUniform(name, v.x, v.y, v.z);
}

void ShaderProgramResource::setUniform(const char *name, const glm::vec4 &v)
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgramResource::setUniform(const char *name, const glm::vec2 &v)
{
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void ShaderProgramResource::setUniform(const char *name, const glm::mat4 &m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderProgramResource::setUniform(const char *name, const glm::mat3 &m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderProgramResource::setUniform(const char *name, float val)
{
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, val);
}

void ShaderProgramResource::setUniform(const char *name, int val)
{
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

void ShaderProgramResource::setUniform(const char *name, const uint64_t &val) {
    GLint loc = getUniformLocation(name);
    glUniformHandleui64ARB(loc, val);
}

void ShaderProgramResource::setUniform(const char *name, unsigned int val)
{
    GLint loc = getUniformLocation(name);
    glUniform1ui(loc, val);
}

void ShaderProgramResource::setUniform(const char *name, bool val)
{
    int loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

int ShaderProgramResource::getUniformLocation(const char *name)
{
    if (!isReady()) {
        return 0;
    }

    auto pos = m_uniformLocations.find(name);

    if (pos == m_uniformLocations.end())
    {
        m_uniformLocations[name] = glGetUniformLocation(m_ProgramId, name);
    }

    return m_uniformLocations[name];
}