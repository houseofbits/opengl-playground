#pragma once
#include <string>

class BaseClass {
public:
    typedef unsigned int IdType;

    virtual ~BaseClass() = default;

    virtual std::string getTypeName() { return "BaseClass"; }
    virtual IdType getTypeId() { return 0; }
    static std::string TypeName() { return "BaseClass"; }
    static IdType TypeId() { return 0; }
};

class ObjectTypeCounter {
public:
    inline static unsigned int count = 0;

    static unsigned int get(unsigned int &i) {
        if (i == 0) {
            count++;
            i = count;
        }

        return i;
    }
};

#define TYPE_DEFINITION(classname)                          \
private:                                                    \
    inline static IdType typeId = 0;                        \
    inline static std::string typeName = #classname;        \
                                                            \
public:                                                     \
    std::string getTypeName() override { return typeName; } \
    IdType getTypeId() override {                           \
        return ObjectTypeCounter::get(classname::typeId);   \
    }                                                       \
    static std::string TypeName() {                         \
        return classname::typeName;                         \
    }                                                       \
    static IdType TypeId() {                                \
        return ObjectTypeCounter::get(classname::typeId);   \
    }
