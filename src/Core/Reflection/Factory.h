#pragma once

#include "../Helper/Log.h"
#include <map>
#include <string>
#include <vector>

template<class G>
class Factory {
private:
    class VObjectConstructor {
    public:
        virtual ~VObjectConstructor() = default;

        virtual G *createInstance() { return nullptr; }
    };

    template<class T>
    class RObjectConstructor final : public VObjectConstructor {
    public:
        G *createInstance() override { return new T(); }
    };

public:
    Factory() = default;
    virtual ~Factory() = default;

    template<class T>
    void registerName(std::string name) {
        if (m_Constructors.count(name) == 0) {
            m_Constructors[name] = new RObjectConstructor<T>();
        } else {
            Log::error("Name is already registered " + name);
        }
    }

    G *createInstance(std::string name) {
        typename std::map<std::string, VObjectConstructor *>::iterator it = m_Constructors.find(name);

        if (it != m_Constructors.end()) {
            return (*it).second->createInstance();
        }

        return nullptr;
    }

    std::vector<std::string> getNames() {
        std::vector<std::string> keys;
        for(auto const& values: m_Constructors)
            keys.push_back(values.first);

        return keys;
    }

private:
    std::map<std::string, VObjectConstructor *> m_Constructors;
};