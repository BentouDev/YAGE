#ifndef YAGE_RTTI_STORAGE_H
#define YAGE_RTTI_STORAGE_H

namespace RTTI
{
    class ClassInfo;
    class EnumInfo;

    class IStorage
    {
    public:
		virtual ~IStorage() {}
        virtual void RegisterType(const std::type_info& type_info, ClassInfo* class_info) = 0;
        virtual void RegisterType(const std::type_info& type_info, EnumInfo* enum_info) = 0;
    };
}

#endif // !YAGE_RTTI_STORAGE_H