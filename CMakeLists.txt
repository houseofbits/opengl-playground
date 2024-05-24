cmake_minimum_required(VERSION 3.16.3)
project(SDLWindow VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${sources}
        src/Renderer/Renderer/StaticGeometryRenderer.cpp
        src/Renderer/Renderer/StaticGeometryRenderer.h
        src/Renderer/Renderer/BaseRenderer.cpp
        src/Renderer/Renderer/BaseRenderer.h
        src/Components/SceneLoader.cpp
        src/Components/SceneLoader.h
        src/Tests/Rendering/TestTangentSpaceCalculation.h
        src/Tests/Rendering/TestNormalMapping.h
        src/Tests/Rendering/TestMipMapping.h
        src/Renderer/Texture/TextureCube.cpp
        src/Renderer/Texture/TextureCube.h
        src/Helper/StringUtils.h
        src/Renderer/Texture/TextureCubeRenderTarget.cpp
        src/Renderer/Texture/TextureCubeRenderTarget.h
        src/Renderer/Renderer/ReflectionProbeRenderer.cpp
        src/Renderer/Renderer/ReflectionProbeRenderer.h
        src/Renderer/Renderer/SkyBoxRenderer.cpp
        src/Renderer/Renderer/SkyBoxRenderer.h
        src/Renderer/Common/ProjectionView.cpp
        src/Renderer/Common/ProjectionView.h
        src/Renderer/Camera/CameraController.cpp
        src/Renderer/Camera/CameraController.h
)

target_link_libraries(SDLWindow SDL2main SDL2 GL)

find_package(GLEW REQUIRED)
target_link_libraries(SDLWindow GLEW::GLEW)

find_package(glm REQUIRED)
target_link_libraries(SDLWindow glm::glm)

target_link_libraries(SDLWindow ${CMAKE_SOURCE_DIR}/libs/tinygltf/build/libtinygltf.a)

target_compile_features(SDLWindow PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
include(CPack)
