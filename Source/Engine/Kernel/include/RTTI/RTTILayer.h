#ifndef YAGE_RTTI_LAYER_H
#define YAGE_RTTI_LAYER_H

#include <typeinfo>
#include <Utils/Defines.h>

namespace RTTI
{
	class IRegister;
	class Register;

	class TypeInfo;
	class EnumInfo;
	class ClassInfo;

    enum class ClassResolvePolicy
    {
        Strict,
        Permissive
    };

    // Derived by engine layer, game lib layer and game script layer
    class YAGE_API ILayer
    {
		friend class IRegister;
		friend class Register;

	protected:
        explicit ILayer()
        { }

		virtual bool TryGetType(const std::type_info&, TypeInfo*& ptr) = 0;
		virtual bool TryGetEnum(const std::type_info&, EnumInfo*& ptr) = 0;
		virtual bool TryGetClass(const std::type_info&, ClassInfo*& ptr) = 0;

    public:
        virtual ~ILayer()
        { }

        bool Load(RTTI::IRegister& rtti, ClassResolvePolicy policy);
        bool Unload();
        bool IsLoaded() const;

    protected:
        virtual bool OnLoad(RTTI::IRegister& rtti) = 0;
        virtual bool OnUnload() = 0;

        bool _loaded;
    };
}

#endif // !YAGE_RTTI_LAYER_H