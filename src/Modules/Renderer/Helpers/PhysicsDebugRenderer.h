#pragma once

#include "Jolt/Jolt.h"
#include "Jolt/Renderer/DebugRenderer.h"
#include "Jolt/Geometry/Triangle.h"
#include "../../../Renderer/Model/VertexArray.h"
#include "../Resources/ShaderProgramResource.h"

class PhysicsDebugRenderer final : public JPH::DebugRenderer {
public:
    PhysicsDebugRenderer();

    ~PhysicsDebugRenderer() override;

    void init();

    void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo,
                  JPH::ColorArg inColor) override;

    void
    DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3,
                 JPH::ColorArg inColor,
                 ECastShadow inCastShadow) override;

    Batch CreateTriangleBatch(const Triangle *inTriangles,
                              int inTriangleCount) override;

    Batch CreateTriangleBatch(const Vertex *inVertices, int inVertexCount,
                              const JPH::uint32 *inIndices,
                              int inIndexCount) override;

    void DrawGeometry(JPH::RMat44Arg inModelMatrix,
                      const JPH::AABox &inWorldSpaceBounds,
                      float inLODScaleSq, JPH::ColorArg inModelColor,
                      const GeometryRef &inGeometry, ECullMode inCullMode,
                      ECastShadow inCastShadow,
                      EDrawMode inDrawMode) override;

    void DrawText3D(JPH::RVec3Arg inPosition,
                    const JPH::string_view &inString,
                    JPH::ColorArg inColor, float inHeight) override;

    static PhysicsDebugRenderer* getInstance() {
        if (sInstance == nullptr) {
            sInstance = new PhysicsDebugRenderer();
        }
        return dynamic_cast<PhysicsDebugRenderer *>(sInstance);
    }


    ShaderProgramResource* m_shader;
};

class ThatIHaveToMake : public JPH::RefTarget<ThatIHaveToMake> {

};

class TriangleData : public JPH::RefTargetVirtual, public ThatIHaveToMake {
public:
    TriangleData();

    void AddRef() override { ThatIHaveToMake::AddRef(); }
    void Release() override { if (--mRefCount == 0) delete this; }

    VertexArray m_vbo;
};