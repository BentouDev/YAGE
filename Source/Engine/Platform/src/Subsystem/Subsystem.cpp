#include "Platform/Subsystem/ISubsystem.h"
#include "Platform/Logger.h"
#include "SubsystemDefinition.h"
#include <EASTL/hash_map.h>

namespace yage::platform
{
	namespace detail
	{
		ISubsystem* _selectedSubsystem = nullptr;
		detail::define_subsystem* _selectedDef = nullptr;

		using TSubsystemCache = eastl::hash_map<eastl::string, define_subsystem*>;

		TSubsystemCache& getCache()
		{
			static TSubsystemCache _known_subsystems{};
			return _known_subsystems;
		}

		define_subsystem::define_subsystem(Utils::CompileString name, TCreator* creator, TDeleter* deleter)
			: _create(creator), _destroy(deleter)
		{
			getCache()[name.c_str()] = this;
		}
	}

	bool initialize(const eastl::string& name, Memory::IMemoryBlock* block, SSubsystemParams& params)
	{
		auto itr = detail::getCache().find(name);
		if (itr == detail::getCache().end())
		{
			Core::Logger::critical("Unable to create subsystem '{}', shutting down.", name.c_str());
			return false;
		}

		Core::Logger::info("Creating {} subsystem...", name.c_str());

		detail::_selectedDef = (*itr).second;
		detail::_selectedSubsystem = detail::_selectedDef->_create(block, params);

		return getSubsystem().initialize();
	}

	void shutdown()
	{
		if (detail::_selectedSubsystem != nullptr)
		{
			Core::Logger::info("Destroying subsystem...");

			detail::_selectedSubsystem->shutdown();
			detail::_selectedDef->_destroy(detail::_selectedSubsystem);

			detail::_selectedSubsystem = nullptr;
			detail::_selectedDef = nullptr;
		}
	}

	ISubsystem& getSubsystem()
	{
		YAGE_ASSERT(detail::_selectedSubsystem != nullptr, "Fatal Error, no subsystem!");
		return *detail::_selectedSubsystem;
	}
}