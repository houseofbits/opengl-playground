#include "ShaderResource.h"
#include "../../../Core/Helper/Console.h"
#include "../../../Core/Helper/ShaderSourceLoader.h"
#include <GL/glew.h>
#include <vector>

struct ShaderFileExt {
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

ShaderResource::ShaderResource() : Resource(), m_SourceCode(), m_Handle() {
}

Resource::Status ShaderResource::fetchData(ResourceManager&) {
    m_SourceCode = ShaderSourceLoader::load(m_Path);

    return STATUS_DATA_READY;
}

Resource::Status ShaderResource::build() {
    unsigned int type = getShaderType(m_Path);
    m_Handle = glCreateShader(type);
    const char *c_code = m_SourceCode.c_str();
    glShaderSource(m_Handle, 1, &c_code, nullptr);
    glCompileShader(m_Handle);

    if(!checkCompileError()) {
        return STATUS_BUILD_ERROR;
    }

    return STATUS_READY;
}

void ShaderResource::destroy() {
    glDeleteShader(m_Handle);
}

unsigned int ShaderResource::getShaderType(const std::string &filename) {
    size_t loc = filename.find_last_of('.');
    std::string ext = filename.substr(loc + 1, std::string::npos);

    if (ext.empty()) {
        return GL_VERTEX_SHADER;
    }

    for (auto &shadeFilerExtension: shadeFilerExtensions) {
        if (ext == shadeFilerExtension.ext) {
            return shadeFilerExtension.type;
        }
    }

    return GL_VERTEX_SHADER;
}

bool ShaderResource::checkCompileError()
{
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_Handle, 1024, nullptr, infoLog);
        std::cout << m_Path << std::endl;
        std::cout << "COMPILATION FAILED " << infoLog << std::endl;
        std::cout << "================================================================" << std::endl;

        int i = 0;
        std::istringstream iss(m_SourceCode);
        for (std::string line; std::getline(iss, line);)
        {
            std::cout << i << ": " << FOREGRN << line << RESETTEXT << std::endl;
            i++;
        }

        std::cout << "================================================================" << std::endl;

        return false;
    }

    return true;
}