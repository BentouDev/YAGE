#ifndef YAGE_RTTI_STORAGE_H
#define YAGE_RTTI_STORAGE_H

namespace RTTI
{
    class ClassInfo;
    class EnumInfo;

    class IStorage
    {
    public:
        virtual void RegisterType(ClassInfo* info) = 0;
        virtual void RegisterType(EnumInfo* info) = 0;
    };
}

#endif // !YAGE_RTTI_STORAGE_H