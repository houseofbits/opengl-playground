#pragma once
#include <string>

class BaseClass {
public:
    virtual std::string getTypeName() { return "BaseClass"; }
    virtual unsigned int getTypeId() { return 0; }
    static std::string TypeName() { return "BaseClass"; }
    static unsigned int TypeId() { return 0; }
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

#define TYPE_DEFINITION(classname)                         \
private:                                                   \
    inline static unsigned int typeId = 0;                 \
    inline static std::string typeName = #classname;       \
                                                           \
public:                                                    \
    virtual std::string getTypeName() { return typeName; } \
    virtual unsigned int getTypeId() {                     \
        return ObjectTypeCounter::get(classname::typeId);  \
    }                                                      \
    static std::string TypeName() {                        \
        return classname::typeName;                        \
    }                                                      \
    static unsigned int TypeId() {                         \
        return ObjectTypeCounter::get(classname::typeId);  \
    }
