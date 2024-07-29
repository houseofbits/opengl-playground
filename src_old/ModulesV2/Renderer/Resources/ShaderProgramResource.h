#pragma once

#include "../../../CoreV2/API.h"
#include "ShaderResource.h"
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <map>
#include <vector>

class ShaderProgramResource : public Resource {
public:
    ShaderProgramResource();

    Resource::Status fetchData(ResourceManager&) override;
    Resource::Status build() override;
    void destroy() override;

    [[nodiscard]] bool checkLinkingError() const;

    void use();
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
    int getUniformLocation(const char *name);

    std::vector<ResourceHandle<ShaderResource>> m_ShaderHandles;
    unsigned int m_ProgramId;
    std::map<std::string, int> m_uniformLocations;
};
