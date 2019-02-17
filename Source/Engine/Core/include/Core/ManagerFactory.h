#ifndef YAGE_MANAGER_FACTORY_H
#define YAGE_MANAGER_FACTORY_H

#include <cstdint>
#include <type_traits>
#include <Core/IManager.h>
#include <Utils/OwnedPtr.h>
#include <Core/MemoryModule.h>

namespace Core
{
	class Engine;
}

namespace yage
{
	template <typename T>
	void CreateManager(Core::Engine& engine, Utils::owned_ptr<T>& ptr, std::size_t memorySize) noexcept
	{
		static_assert(std::is_base_of<Core::IManager, T>::value,
			"Engine : Cannot create manager that doesnt derive from Core::IManager!");

		Memory::IMemoryBlock& memoryBlock = engine.MemoryModule->requestMemoryBlock(memorySize, T::GetStaticClassName());
		T* manager = YAGE_CREATE_NEW(engine.MemoryModule->masterBlock(), T)(engine, memoryBlock);

		ptr.reset(manager);

		engine.RegisterManager(std::move(ptr.template borrow<Core::IManager>()));
	}

	template <typename T>
	void UnregisterManager(Core::Engine& engine, Utils::owned_ptr<T>& ptr)
	{
		static_assert(std::is_base_of<Core::IManager, T>::value,
			"Engine : Cannot unregister manager that doesnt derive from Core::IManager!");
		engine.UnregisterManager(ptr);
	}

	template <typename T>
	std::function<void(void*)> GetManagerDeleter(Core::Engine* engine)
	{
		static_assert(std::is_base_of<Core::IManager, T>::value,
			"Engine : Cannot destroy manager that doesnt derive from Core::IManager!");
		return std::function<void(void*)>
		{
			[engine](void* ptr) {
				T* ptr_t = reinterpret_cast<T*>(ptr);
				Memory::Delete(engine->MemoryModule->masterBlock(), ptr_t);
			}
		};
	}
}

#endif // !YAGE_MANAGER_FACTORY_H