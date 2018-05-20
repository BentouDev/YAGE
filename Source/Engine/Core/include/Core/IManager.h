//
// Created by bentoo on 14.11.16.
//

#ifndef GAME_IMANAGER_H
#define GAME_IMANAGER_H

#include <Utils/Handle.h>
#include <Utils/MemoryBlock.h>

#define MANAGER(Clazz) \
private: \
	static constexpr const char* ClazzName = #Clazz; \
public: \
	static inline constexpr const char* const GetStaticClassName() \
	{ return ClazzName; }

namespace Core
{
	class Engine;

	class IManager
	{
	protected:
		explicit IManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~IManager() noexcept;

		Core::Engine&			_engine;
		Memory::IMemoryBlock&	_memory;

	public:
		IManager(IManager&&) = delete;
		IManager(const IManager&) = delete;
		IManager& operator=(IManager&&) = delete;
		IManager& operator=(const IManager&) = delete;

		Memory::IMemoryBlock& getMemoryBlock() const
		{ return _memory; }
	};
}

#endif //GAME_IMANAGER_H
