#pragma once

#include "BaseUniform.h"
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <any>
#include <string>

class UniformsRegistry {
public:
    UniformsRegistry();

//     template<typename T>
//     void add(std::string name, const BaseUniform<T> &uniform) {
//         auto &map = getOrCreateMap<T>();
//         map[name] = &uniform;
//     }
//
//     template<typename T>
//     BaseUniform<T> *get(std::string name) {
//         auto &map = getOrCreateMap<T>();
//
//         return map[name];
//     }
//
// private:
//     std::unordered_map<std::type_index, std::any> storage;
//
//     template<typename T>
//     using NamedUniformMap = std::unordered_map<std::string, BaseUniform<T> *>;
//
//     template<typename T>
//     NamedUniformMap<T> &getOrCreateMap() {
//         const auto type = std::type_index(typeid(T));
//
//         if (const auto iter = storage.find(type); iter == storage.end()) {
//             storage[type] = NamedUniformMap<T>{};
//         }
//
//         return std::any_cast<NamedUniformMap<T> &>(storage[type]);
//     }
};
