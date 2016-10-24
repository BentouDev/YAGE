//
// Created by bentoo on 10/20/16.
//

#ifndef GAME_RENDERBATCH_H
#define GAME_RENDERBATCH_H

#include <cstdint>
#include <Utils/List.h>
#include <Utils/DefaultTrait.h>

#include "OpenGl/OpenGLBuffers.h"

namespace Core
{
	class Mesh;
	class MeshData;
}

namespace Gfx
{
	class RenderPass;

	class RenderBatch
	{
	public:
		struct SubmeshInfo
		{
			std::uint32_t baseVertex;
			std::uint32_t count;
			std::uint32_t indexType;
			std::uint32_t vboOffset;
		};

		struct MaterialInfo
		{
			// Material handle
			Utils::List<SubmeshInfo> submeshes;

			explicit inline MaterialInfo(Memory::IMemoryBlock& memory)
				: submeshes(memory)
			{

			}
		};

	private:
		const Core::MeshData& _scheme;

		Utils::Handle<RenderBatch> Handle;

		OpenGL::VAO* _vao;
		OpenGL::VBO* _vbo;
		OpenGL::VBO* _ibo;

		const RenderPass& _parentBucket;
		Utils::List<MaterialInfo> _materialBatches;

		size_t _size;
		size_t _usedSize;

		inline void clear()
		{
			_materialBatches.clear();
			_usedSize = 0;
		}

	public:
		explicit inline RenderBatch(const RenderPass& parentBucket, const Core::MeshData& scheme) noexcept;

		virtual ~RenderBatch() noexcept;

		inline bool hasSize(size_t size) const noexcept { return _size - _usedSize > size; }

		inline const OpenGL::VBO& getVBO() const noexcept { return *_vbo; }

		inline const OpenGL::VBO& getIBO() const noexcept { return *_ibo; }

		inline const OpenGL::VAO& getVAO() const noexcept { return *_vao; }

		inline const Utils::List<MaterialInfo>& getMaterialBatches() const noexcept { return _materialBatches; }

		void updateMesh(uint32_t index, const Core::Mesh& mesh);

		void addMesh(const Core::Mesh& mesh);

		void init();

		void swap(RenderBatch& other);

		void cleanUp();
	};

	class RenderBatchTrait : public Utils::DefaultTrait<RenderBatch> { };
}

#endif //GAME_RENDERBATCH_H
