#ifndef YAGE_RTTI_DEFINE_LAYER_H
#define YAGE_RTTI_DEFINE_LAYER_H

#include <typeinfo>
#include <type_traits>
#include "RTTILayer.h"
#include "Utils/MemoryBlock.h"
#include "Utils/CompileString.h"

namespace RTTI
{
	class IRegister;
}

namespace kernel::rtti::detail
{
	using TCreator = RTTI::ILayer* (Memory::IMemoryBlock* block);
	using TDeleter = void(Memory::IMemoryBlock* block, RTTI::ILayer* subsystem);
	using TLoader  = void(RTTI::IRegister& rtti);

	struct YAGE_API define_layer
	{
		TCreator* _create;
		TDeleter* _delete;
		TLoader* _load;

		virtual ~define_layer();

	protected:
		define_layer(Utils::CompileString name, const std::type_info& info, TCreator* create, TDeleter* del, TLoader* load);
	};

	template <typename T>
	struct define_rtti_layer_impl : public define_layer
	{
		static_assert(std::is_base_of<RTTI::ILayer, T>::value, "Must derive from ILayer");

		static RTTI::ILayer* Create(Memory::IMemoryBlock* block)
		{
			return YAGE_CREATE_NEW((*block), T) ();
		}

		static void Destroy(Memory::IMemoryBlock* block, RTTI::ILayer* sys)
		{
			Memory::Delete((*block), sys);
		}

		define_rtti_layer_impl(Utils::CompileString name, TLoader* proc)
			: define_layer(name, typeid(T), &define_rtti_layer_impl::Create, &define_rtti_layer_impl::Destroy, proc)
		{ }

		~define_rtti_layer_impl()
		{ }
	};
}

#define __PUBLISH_RTTI_LAYER( clazz_name, clazz, proc_ptr ) \
	kernel::rtti::detail::define_rtti_layer_impl<clazz> clazz_name {#clazz, proc_ptr};

#define PUBLISH_RTTI_LAYER( clazz, proc_ptr ) \
	__PUBLISH_RTTI_LAYER ( YAGE_MAKE_UNIQUE(layer_helper_), clazz, proc_ptr )

#endif//YAGE_RTTI_DEFINE_LAYER_H