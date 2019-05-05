#ifndef BASE_LAYER_H
#define BASE_LAYER_H

#include <RTTI/RTTILayer.h>
#include <RTTI/RTTIStorage.h>
#include <Utils/List.h>
#include <EASTL/hash_map.h>

namespace RTTI
{
	using TLoader = void(RTTI::IRegister& rtti);

	class BaseLayer : public ILayer, public IStorage
    {
		friend void InitializeLoader(BaseLayer*,TLoader*);

		TLoader* _loader;

	protected:
        explicit BaseLayer();
        virtual ~BaseLayer();

        // ILayer
        virtual bool OnUnload() override;
        virtual bool OnLoad(RTTI::IRegister& rtti) override;

		virtual bool TryGetType(const std::type_info&, TypeInfo*& ptr) override;
		virtual bool TryGetEnum(const std::type_info&, EnumInfo*& ptr) override;
		virtual bool TryGetClass(const std::type_info&, ClassInfo*& ptr) override;

        // IStorage
        virtual void RegisterType(const std::type_info& info, ClassInfo* clazz) override;
        virtual void RegisterType(const std::type_info& info, EnumInfo*  enumz) override;

        eastl::hash_map<size_t, ClassInfo*> Classes;
		eastl::hash_map<size_t, EnumInfo*>  Enums;
    };
}

#endif //BASE_LAYER_H