#include "ShaderBuilder.h"
#include "../../Core/Helper/Console.h"
#include "../../Core/Helper/Log.h"
#include "../../Core/Helper/ShaderSourceLoader.h"
#include "../Camera/Camera.h"
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

ShaderBuilder::ShaderBuilder() {
}

unsigned int ShaderBuilder::getShaderType(const std::string &filename) {
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

void ShaderBuilder::loadFromFile(const std::string &filepath) {
    m_source[getShaderType(filepath)] = {filepath, ShaderSourceLoader::load(filepath)};
}

void ShaderBuilder::loadFromString(const unsigned int shaderGLType, const std::string &source) {
    m_source[shaderGLType] = {"SOURCE", source};
}

std::optional<unsigned int> ShaderBuilder::compile() {
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
    }

    auto programId = 0;
    if (!hasError) {
        programId = glCreateProgram();

        for (const auto shaderId: shaderIDs) {
            glAttachShader(programId, shaderId);
        }

        glLinkProgram(programId);
        checkLinkingError(programId);
    }

    for (const auto shaderId: shaderIDs) {
        glDeleteShader(shaderId);
    }

    m_source.clear();

    if (programId) {
        return programId;
    }
    return std::nullopt;
}

unsigned int ShaderBuilder::loadShaderSource(const unsigned int type, const std::string &source,
                                             const std::string &filename) {
    const GLuint shaderHandle = glCreateShader(type);

    const char *c_code = source.c_str();

    glShaderSource(shaderHandle, 1, &c_code, nullptr);
    glCompileShader(shaderHandle);

    if (!checkCompileError(shaderHandle, filename, source)) {
        return 0;
    }

    return shaderHandle;
}

bool ShaderBuilder::checkCompileError(const unsigned int shader, const std::string &name, const std::string &source) {
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

void ShaderBuilder::checkLinkingError(const unsigned int shader) {
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
        std::cout << "LINKING FAILED " << infoLog << std::endl;
    }
}
