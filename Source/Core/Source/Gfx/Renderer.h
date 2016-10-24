//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "../Context.h"
#include "RenderPass.h"

#include <Utils/BorrowedPtr.h>
#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <Utils/Container.h>

namespace Logic
{
	class Scene;
	class RenderingComponent;
}

namespace Core
{
	class Engine;
	class Mesh;
	class MeshData;
}

namespace Gfx
{
	class BaseApi;

	class Renderer
	{
	protected:
		Memory::IMemoryBlock& 	_memory;
		Utils::List<RenderPass>	_buckets;

		// Would provide a nice key for batch that may be reused!
	//	Utils::Container<RenderBatchTrait> _batches;

		Core::Engine& _engine;
		Core::Context _context;

	public:
		Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory);

		void updateMesh(Core::Mesh& mesh);
		RenderBatch& getBucket(Core::MeshData& meshData);

		// TODO: refactor to handle and container
	//	void recreateBatch(RenderBatchTrait::handle handle);
		void updateComponent(Logic::RenderingComponent& comp);

		void draw();
	};
}

#endif //GAME_RENDERER_H
