#include "Shader.h"
#include "../Helper/Logging.h"
#include "../Helper/Console.h"
#include "../Helper/ShaderSourceLoader.h"
#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

struct ShaderFileExt
{
    const char *ext;
    unsigned int type;
};

std::vector<ShaderFileExt> shadeFilerExtensions =
    {
        {"vs", GL_VERTEX_SHADER},
        {"vert", GL_VERTEX_SHADER},
        {"gs", GL_GEOMETRY_SHADER},
        {"geom", GL_GEOMETRY_SHADER},
        {"tcs", GL_TESS_CONTROL_SHADER},
        {"tes", GL_TESS_EVALUATION_SHADER},
        {"fs", GL_FRAGMENT_SHADER},
        {"frag", GL_FRAGMENT_SHADER},
        {"cs", GL_COMPUTE_SHADER}};

unsigned int Shader::getShaderType(std::string filename)
{
    size_t loc = filename.find_last_of('.');
    std::string ext = filename.substr(loc + 1, std::string::npos);

    if (ext.length() == 0)
    {
        return GL_VERTEX_SHADER;
    }

    for (int i = 0; i < shadeFilerExtensions.size(); i++)
    {
        if (ext == shadeFilerExtensions[i].ext)
        {
            return shadeFilerExtensions[i].type;
        }
    }

    return GL_VERTEX_SHADER;
}

unsigned int Shader::loadShader(std::string filename)
{
    unsigned int type = getShaderType(filename);

    std::string sourceStr = ShaderSourceLoader::load(filename);
    const char *c_code = sourceStr.c_str();

    GLuint shaderHandle = glCreateShader(type);
    glShaderSource(shaderHandle, 1, &c_code, NULL);
    glCompileShader(shaderHandle);

    checkCompileError(shaderHandle, filename, sourceStr);

    return shaderHandle;
}

void Shader::loadProgram(std::string vertexProgramFileName, std::string fragmentProgramFileName, std::string geometryProgramFileName)
{
    unsigned int vertexShaderId = loadShader(vertexProgramFileName);
    unsigned int fragmentShaderId = loadShader(fragmentProgramFileName);
    unsigned int geometryShaderId = 0;

    if (geometryProgramFileName.size() > 0)
    {
        geometryShaderId = loadShader(geometryProgramFileName);
    }

    if (!vertexShaderId || !fragmentShaderId)
    {
        return;
    }

    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    if (geometryShaderId != 0)
    {
        glAttachShader(programId, geometryShaderId);
    }
    glLinkProgram(programId);

    checkLinkingError(programId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::use()
{
    glUseProgram(programId);
}

void Shader::setUniform(const char *name, float x, float y, float z)
{
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, x, y, z);
}

void Shader::setUniform(const char *name, const glm::vec3 &v)
{
    this->setUniform(name, v.x, v.y, v.z);
}

void Shader::setUniform(const char *name, const glm::vec4 &v)
{
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const char *name, const glm::vec2 &v)
{
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void Shader::setUniform(const char *name, const glm::mat4 &m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, const glm::mat3 &m)
{
    GLint loc = getUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char *name, float val)
{
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, val);
}

void Shader::setUniform(const char *name, int val)
{
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

void Shader::setUniform(const char *name, unsigned int val)
{
    GLint loc = getUniformLocation(name);
    glUniform1ui(loc, val);
}

void Shader::setUniform(const char *name, bool val)
{
    int loc = getUniformLocation(name);
    glUniform1i(loc, val);
}

int Shader::getUniformLocation(const char *name)
{
    auto pos = uniformLocations.find(name);

    if (pos == uniformLocations.end())
    {
        uniformLocations[name] = glGetUniformLocation(programId, name);
    }

    return uniformLocations[name];
}

void Shader::checkCompileError(unsigned int shader, std::string name, std::string source)
{
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << name << std::endl;
        std::cout << "COMPILATION FAILED " << infoLog << std::endl;
        std::cout << "================================================================" << std::endl;
        // std::cout << source << std::endl;

        int i = 0;
        std::string line;
        std::istringstream iss(source);
        for (std::string line; std::getline(iss, line);)
        {
            std::cout << i << ": " << FOREGRN << line << RESETTEXT << std::endl;
            i++;
        }

        std::cout << "================================================================" << std::endl;
    }
}

void Shader::checkLinkingError(unsigned int shader)
{
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "LINKING FAILED " << infoLog << std::endl;
    }
}
