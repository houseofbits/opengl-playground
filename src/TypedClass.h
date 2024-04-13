#pragma once
#include <string>

class BaseClass
{
public:
    virtual std::string getTypeName() { return "BaseClass"; }
    virtual unsigned int getTypeId() { return 0; }
    static std::string TypeName() { return "BaseClass"; }
    static unsigned int TypeId() { return 0; }
};

static unsigned int globalObjectTypeCounter = 0;

#define TYPE_DEFINITION(classname)                                 \
private:                                                           \
    inline static unsigned int typeId = globalObjectTypeCounter++; \
    inline static std::string typeName = #classname;               \
                                                                   \
public:                                                            \
    virtual std::string getTypeName() { return typeName; }         \
    virtual unsigned int getTypeId()                               \
    {                                                              \
        return classname::typeId;                                  \
    }                                                              \
    static std::string TypeName() { return classname::typeName; }  \
    static unsigned int TypeId()                                   \
    {                                                              \
        return classname::typeId;                                  \
    }
