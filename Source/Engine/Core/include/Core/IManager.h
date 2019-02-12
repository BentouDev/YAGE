//
// Created by bentoo on 14.11.16.
//

#ifndef GAME_IMANAGER_H
#define GAME_IMANAGER_H

#include <Utils/Handle.h>

namespace Memory
{
	class IMemoryBlock;
}

#define MANAGER(Clazz) \
public: \
	static inline constexpr const char* const GetStaticClassName() \
	{ return ClazzName; } \
	virtual const char* GetClassNameA() const override { return GetStaticClassName(); } \
private: \
	static constexpr const char* ClazzName = #Clazz;

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

		virtual const char* GetClassNameA() const = 0;
	};
}

#endif //GAME_IMANAGER_H
