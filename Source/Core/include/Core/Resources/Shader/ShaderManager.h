//
// Created by bentoo on 09.11.16.
//

#ifndef GAME_SHADERMANAGER_H
#define GAME_SHADERMANAGER_H

#include <Utils/MemoryBlock.h>
#include <Utils/Handle.h>
#include <Utils/Container.h>

#include "Shader.h"

namespace Core
{
	class Engine;
}

namespace Resources
{
	class ShaderManager
	{
	public:
		using handle_t = Utils::Handle<Gfx::ShaderProgram>;

	protected:
		Core::Engine&			_engine;
		Memory::IMemoryBlock&	_memory;

		Utils::Container<Gfx::ShaderProgTrait> _shadersContainer;

		void dispose(Gfx::ShaderProgram&);

	public:
		explicit ShaderManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~ShaderManager();

		void dispose(handle_t);

		inline handle_t createNew()
		{ return _shadersContainer.create(); }

		inline Gfx::ShaderProgram& get(handle_t handle) const
		{ return _shadersContainer.get(handle); }

		Gfx::ShaderProgram* tryGet(handle_t) const;

		inline bool contains(handle_t handle) const
		{ return _shadersContainer.contains(handle); }
	};
}

#endif //GAME_SHADERMANAGER_H
