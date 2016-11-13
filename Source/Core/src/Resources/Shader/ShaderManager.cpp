//
// Created by bentoo on 09.11.16.
//

#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

namespace Resources
{
	ShaderManager::ShaderManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
	 : _engine(engine), _memory(memory), _shadersContainer(_memory)
	{
		_engine.Logger->Default->info("Created shader manager with capacity {}", _shadersContainer.capacity());
	}

	ShaderManager::~ShaderManager()
	{

	}

	void ShaderManager::dispose(handle_t handle)
	{
		if(_shadersContainer.contains(handle))
			dispose(_shadersContainer.get(handle));
	}

	void ShaderManager::dispose(Gfx::ShaderProgram&)
	{

	}

	Gfx::ShaderProgram* ShaderManager::tryGet(handle_t handle) const
	{
		Gfx::ShaderProgram* ptr = nullptr;
		if(_shadersContainer.contains(handle))
		{
			ptr = &get(handle);
		}
		return ptr;
	}
}
