#pragma once

#include "../Helper/Log.h"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

template<class G>
class Factory {
private:
    class VObjectConstructor {
    public:
        virtual G *createInstance() { return 0; };
    };

    template<class T>
    class RObjectConstructor : public VObjectConstructor {
    public:
        G *createInstance() { return new T(); };
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