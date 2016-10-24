//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_RENDERBUCKET_H
#define GAME_RENDERBUCKET_H

#include <cstdint>
#include <Utils/List.h>
#include "RenderBatch.h"
#include "RenderTarget.h"

namespace Core
{
	class Mesh;
	class MeshData;
}

namespace Gfx
{
	class Submesh;

	class RenderPass
	{
		Gfx::RenderTarget*			_target;
		Memory::IMemoryBlock& 		_memory;
		Utils::List<RenderBatch>	_batches;

	public:
		inline explicit RenderPass(Memory::IMemoryBlock& memory)
			: _memory(memory), _batches(memory)
		{

		}

		inline Memory::IMemoryBlock& getMemory() const noexcept { return _memory; }

		inline const Utils::List<RenderBatch>& getBatches() const noexcept { return _batches; }

		inline void setRenderTarget(Gfx::RenderTarget& target)
		{ _target = &target; }
	};
}

#endif //GAME_RENDERBUCKET_H
