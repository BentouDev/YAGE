#ifndef YAGE_SUBSYSTEM_DEF_H
#define YAGE_SUBSYSTEM_DEF_H

#include <Utils/CompileString.h>
#include <Utils/MemoryBlock.h>
#include <EASTL/string.h>
#include <type_traits>

namespace yage::platform
{
	struct SSubsystemParams;
	class ISubsystem;
}

namespace yage::platform::detail
{
	using TCreator = ISubsystem* (Memory::IMemoryBlock* block, SSubsystemParams& params);
	using TDeleter = void(ISubsystem* subsystem);

	struct define_subsystem
	{
		TCreator* _create;
		TDeleter* _destroy;

	protected:
		define_subsystem(Utils::CompileString name, TCreator* create, TDeleter* destroy);
	};

	template <typename T>
	struct define_subsystem_impl : public define_subsystem
	{
		static_assert(std::is_base_of<ISubsystem, T>::value, "Must derive from ISubsystem");

		YAGE_API static ISubsystem* Create(Memory::IMemoryBlock* block, SSubsystemParams& params)
		{
			return YAGE_CREATE_NEW((*block), T) (block, params);
		}

		YAGE_API static void Destroy(ISubsystem* sys)
		{
			Memory::Delete(*sys->owningBlock(), sys);
		}

		constexpr define_subsystem_impl(Utils::CompileString name)
			: define_subsystem(name, &define_subsystem_impl::Create, &define_subsystem_impl::Destroy)
		{ }
	};
}

#define PUBLISH_SUBSYSTEM_DETAIL(varname, name) \
	yage::platform::detail::define_subsystem_impl<name> YAGE_MAKE_UNIQUE(varname) {#name};

#define PUBLISH_SUBSYSTEM(name) \
    PUBLISH_SUBSYSTEM_DETAIL(YAGE_CONCATENATE(_sys_, name), name)

#endif//YAGE_SUBSYSTEM_DEF_H