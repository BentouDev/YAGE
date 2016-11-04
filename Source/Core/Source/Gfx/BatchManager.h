//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_BATCHMANAGER_H
#define GAME_BATCHMANAGER_H

#include <Utils/List.h>
#include <Utils/Handle.h>
#include <Utils/MemoryBlock.h>
#include <Utils/DefaultTrait.h>
#include <Utils/Container.h>

#include "OpenGl/OpenGLBuffers.h"
#include "MaterialBatch.h"
#include "StaticBuffer.h"

namespace Core
{
	class Engine;
	class Mesh;
	class Material;
}

namespace Gfx
{
	class BatchManager
	{
		Core::Engine&							_engine;
		Memory::IMemoryBlock&					_memory;
		Utils::Container<MaterialBatchTrait>	_materialBatchesContainer;
		Utils::Container<StaticBufferTrait>		_staticBuffersContainer;

		void allocateMesh(MaterialBatch& batch, const Core::Mesh& mesh);
		void findStaticBufferInMaterial();

	public:
		explicit BatchManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~BatchManager() noexcept;

		BatchManager(const BatchManager&) = delete;
		BatchManager(BatchManager&&) = delete;

		MaterialBatch& getBatch(MaterialBatch::handle_t handle);
		MaterialBatchMeshReference allocateMesh(const Core::Mesh& mesh, const Core::Material& materials);
		void allocateMeshes(const Core::Mesh& mesh, Utils::List<Core::Material>& materials, Utils::List<MaterialBatchMeshReference>& result);
		void invalidateBatch(MaterialBatch::handle_t handle);
	};
}

#endif //GAME_BATCHMANAGER_H
