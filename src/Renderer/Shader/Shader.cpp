#include "Shader.h"
#include "../../Core/Helper/Console.h"
#include "../../Core/Helper/Log.h"
#include "../../Core/Helper/ShaderSourceLoader.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ShaderFileExt {
    const char *ext;
    unsigned int type;
};

std::vector<ShaderFileExt> shadeFilerExtensions2 =
{
    {"vs", GL_VERTEX_SHADER},
    {"vert", GL_VERTEX_SHADER},
    {"gs", GL_GEOMETRY_SHADER},
    {"geom", GL_GEOMETRY_SHADER},
    {"tcs", GL_TESS_CONTROL_SHADER},
    {"tes", GL_TESS_EVALUATION_SHADER},
    {"fs", GL_FRAGMENT_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
    {"cs", GL_COMPUTE_SHADER}
};

unsigned int Shader::getShaderType(const std::string &filename) {
    size_t loc = filename.find_last_of('.');
    std::string ext = filename.substr(loc + 1, std::string::npos);

    if (ext.empty()) {
        return GL_VERTEX_SHADER;
    }

    for (auto &i: shadeFilerExtensions2) {
        if (ext == i.ext) {
            return i.type;
        }
    }

    return GL_VERTEX_SHADER;
}

Shader::~Shader() {
    glDeleteShader(programId);
}

void Shader::loadFromFile(const std::string &filepath) {
    m_source[getShaderType(filepath)] = {filepath, ShaderSourceLoader::load(filepath)};
}

void Shader::loadFromString(const unsigned int shaderGLType, const std::string &source) {
    m_source[shaderGLType] = {"SOURCE", source};
}

void Shader::compileAndLink() {
    bool hasError = false;
    std::vector<unsigned int> shaderIDs;
    for (const auto &[type, data]: m_source) {
        const auto [filename, source] = data;

        auto shaderHandle = loadShaderSource(type, source, filename);
        if (!shaderHandle) {
            hasError = true;
            break;
        }
        shaderIDs.push_back(shaderHandle);

        // Log::write(" shader ", shaderHandle, " ", filename);
    }

    if (!hasError) {
        programId = glCreateProgram();

        for (const auto shaderId: shaderIDs) {
            glAttachShader(programId, shaderId);
        }

        glLinkProgram(programId);
        checkLinkingError(programId);

        // Log::write("Build shader ", programId);
    }

    for (const auto shaderId: shaderIDs) {
        glDeleteShader(shaderId);
    }

    m_source.clear();
}

unsigned int Shader::loadShaderSource(const unsigned int type, const std::string &source, const std::string &filename) {
    const GLuint shaderHandle = glCreateShader(type);

    const char *c_code = source.c_str();

    glShaderSource(shaderHandle, 1, &c_code, nullptr);
    glCompileShader(shaderHandle);

    if (!checkCompileError(shaderHandle, filename, source)) {
        return 0;
    }

    return shaderHandle;
}

void Shader::use() const {
    glUseProgram(programId);
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

int Shader::getUniformLocation(const char *name) {
    if (programId == 0) {
        return 0;
    }

    if (const auto pos = uniformLocations.find(name); pos == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(programId, name);
    }

    return uniformLocations[name];
}

bool Shader::checkCompileError(const unsigned int shader, const std::string &name, const std::string &source) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cout << name << std::endl;
        std::cout << "COMPILATION FAILED " << infoLog << std::endl;
        std::cout << "================================================================" << std::endl;

        int i = 0;
        std::istringstream iss(source);
        for (std::string line; std::getline(iss, line);) {
            std::cout << i << ": " << FOREGRN << line << RESETTEXT << std::endl;
            i++;
        }

        std::cout << "================================================================" << std::endl;

        return false;
    }

    return true;
}

void Shader::checkLinkingError(const unsigned int shader) {
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
        std::cout << "LINKING FAILED " << infoLog << std::endl;
    }
}
