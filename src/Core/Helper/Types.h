#pragma once

#include <string_view>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

template<typename T>
std::string type_name() {
    int status;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string name = (status == 0) ? demangled : typeid(T).name();
    std::free(demangled);
    return name;
// #if defined(__clang__)
//     constexpr std::string_view p = __PRETTY_FUNCTION__;
//     // "constexpr std::string_view type_name() [T = MyClass]"
//     constexpr std::string_view key = "T = ";
//     constexpr std::string_view end = "]";
//     auto start = p.find(key) + key.size();
//     auto finish = p.find_last_of(end);
//     return p.substr(start, finish - start);
// #elif defined(__GNUC__)
//     constexpr std::string_view p = __PRETTY_FUNCTION__;
//     // "constexpr std::string_view type_name() [with T = MyClass]"
//     constexpr std::string_view key = "T = ";
//     auto start = p.find(key) + key.size();
//     auto finish = p.find_last_of(']');
//     return p.substr(start, finish - start);
// #elif defined(_MSC_VER)
//     constexpr std::string_view p = __FUNCSIG__;
//     // "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<void>(void)"
//     constexpr std::string_view key = "type_name<";
//     auto start = p.find(key) + key.size();
//     auto finish = p.find_last_of('>');
//     return p.substr(start, finish - start);
// #else
//     return "Unsupported compiler";
// #endif
}

//#include <PhysX/PxPhysics.h>
//#include <PhysX/PxPhysicsAPI.h>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/vec3.hpp>
//
// namespace Types {

//    inline physx::PxVec3 GLMtoPxVec3(const glm::vec3& val) {
//        return {val.x, val.y, val.z};
//    }
//
//    inline physx::PxExtendedVec3 GLMtoPxExtendedVec3(const glm::vec3& val) {
//        return {val.x, val.y, val.z};
//    }
//
//    inline physx::PxTransform GLMtoPxTransform(const glm::mat4& m) {
//        glm::quat rq = glm::quat_cast(m);
//        physx::PxQuat rotation(rq.x, rq.y, rq.z, rq.w);
//        physx::PxTransform transform(m[3].x, m[3].y, m[3].z, rotation);
//
//        return transform;
//    }

// }
