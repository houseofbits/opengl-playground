#include "PhysicsDebugRenderer.h"
#include "../../../Core/Helper/Log.h"
#include "../../Physics/Helpers/PhysicsTypeCast.h"

void PhysicsDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) {
    // Log::warn("PhysicsDebugRenderer::DrawLine not implemented");

    m_wireModel.setColor(PhysicsTypeCast::JPHToGlm(inColor.ToVec4()));
    m_wireModel.addVertex(PhysicsTypeCast::JPHToGlm(inFrom));
    m_wireModel.addVertex(PhysicsTypeCast::JPHToGlm(inTo));
}

JPH::DebugRenderer::Batch PhysicsDebugRenderer::CreateTriangleBatch(const Triangle *inTriangles, int inTriangleCount) {
    auto *triangleData = new TriangleData();

    std::vector<unsigned int> el = {};
    std::vector<float> p{};
    std::vector<float> n{};
    std::vector<float> c{};
    std::vector<float> uv{};
    long indexCount = 0;
    for (int i = 0; i < inTriangleCount; ++i) {
        for (const auto &j: inTriangles[i].mV) {
            p.push_back(j.mPosition.x);
            p.push_back(j.mPosition.y);
            p.push_back(j.mPosition.z);

            n.push_back(j.mNormal.x);
            n.push_back(j.mNormal.y);
            n.push_back(j.mNormal.z);

            c.push_back(static_cast<float>(j.mColor.r) / 255.0f);
            c.push_back(static_cast<float>(j.mColor.g) / 255.0f);
            c.push_back(static_cast<float>(j.mColor.b) / 255.0f);
            c.push_back(static_cast<float>(j.mColor.a) / 255.0f);

            uv.push_back(j.mUV.x);
            uv.push_back(j.mUV.y);

            el.push_back(indexCount);
            indexCount++;
        }
    }

    triangleData->m_vbo.createBuffers(&el, &p, &n, &uv, nullptr, &c);

    return triangleData;
}

JPH::DebugRenderer::Batch
PhysicsDebugRenderer::CreateTriangleBatch(const Vertex *inVertices, int inVertexCount, const JPH::uint32 *inIndices,
                                          int inIndexCount) {
    auto *triangleData = new TriangleData();

    std::vector<unsigned int> el = {};
    std::vector<float> p{};
    std::vector<float> n{};
    std::vector<float> c{};
    std::vector<float> uv{};

    for (int i = 0; i < inIndexCount; ++i) {
        el.push_back(inIndices[i]);
    }

    for (int i = 0; i < inVertexCount; ++i) {
        auto j = inVertices[i];
        p.push_back(j.mPosition.x);
        p.push_back(j.mPosition.y);
        p.push_back(j.mPosition.z);

        n.push_back(j.mNormal.x);
        n.push_back(j.mNormal.y);
        n.push_back(j.mNormal.z);

        c.push_back(static_cast<float>(j.mColor.r) / 255.0f);
        c.push_back(static_cast<float>(j.mColor.g) / 255.0f);
        c.push_back(static_cast<float>(j.mColor.b) / 255.0f);
        c.push_back(static_cast<float>(j.mColor.a) / 255.0f);

        uv.push_back(j.mUV.x);
        uv.push_back(j.mUV.y);
    }

    triangleData->m_vbo.createBuffers(&el, &p, &n, &uv, nullptr, &c);

    return triangleData;
}

void
PhysicsDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox &inWorldSpaceBounds,
                                   float inLODScaleSq,
                                   JPH::ColorArg inModelColor, const GeometryRef &inGeometry, ECullMode inCullMode,
                                   ECastShadow inCastShadow, EDrawMode inDrawMode) {
    if (inGeometry == nullptr) {
        return;
    }

    glm::vec4 modelColor = glm::vec4(
        static_cast<float>(inModelColor.r) / 255.0f,
        static_cast<float>(inModelColor.g) / 255.0f,
        static_cast<float>(inModelColor.b) / 255.0f,
        static_cast<float>(inModelColor.a) / 255.0f
    );

    auto *triangleBatch = dynamic_cast<TriangleData *>(inGeometry->mLODs[0].mTriangleBatch.GetPtr());

    m_shader->setUniform("modelMatrix", PhysicsTypeCast::JPHToGlm(inModelMatrix));
    m_shader->setUniform("modelColor", modelColor);
    triangleBatch->m_vbo.draw();
}

void
PhysicsDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor,
                                   ECastShadow inCastShadow) {
    Log::warn("PhysicsDebugRenderer::DrawTriangle not implemented");
}

void PhysicsDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view &inString, JPH::ColorArg inColor,
                                      float inHeight) {
    Log::warn("PhysicsDebugRenderer::DrawText3D not implemented");
}

PhysicsDebugRenderer::PhysicsDebugRenderer() : DebugRenderer(), m_shader(nullptr) {
    DebugRenderer::Initialize();
}

void PhysicsDebugRenderer::init() {
}

PhysicsDebugRenderer::~PhysicsDebugRenderer() = default;

TriangleData::TriangleData() : RefTargetVirtual(), ThatIHaveToMake(), m_vbo() {
}
