//
// Created by Bentoo on 2017-04-09.
//

#ifndef YAGE_SPRITEBATCHMANAGER_H
#define YAGE_SPRITEBATCHMANAGER_H

#include "Core/IManager.h"
#include "Core/Gfx/OpenGl/OpenGLBuffers.h"

#include <map>
#include <Utils/List.h>

namespace Core
{
	class Material;
}

namespace Gfx
{
	struct SpriteVertex;
	class SpriteBatch;
	class Camera;
	class Renderer;
	class RenderTarget;
	class SpriteBatchManager;

	struct SpriteBuffer
	{
		friend class SpriteBatchManager;

	public:
		static const std::int32_t DEFAULT_BUFFER_SIZE;
		static const std::int32_t MAX_BUFFER_SIZE;

	private:
		void*			_mappedPtr;
		OpenGL::VBO*	_vbo;
		OpenGL::VAO*  	_vao;

	public:
		explicit SpriteBuffer(Memory::IMemoryBlock& memory, std::int32_t size);

		std::int32_t	maximumSize;
		std::int32_t	currentSize;
		std::int32_t	offset;

		void 				clear();
		void 				mapMemory();
		void 				unmapMemory();
		inline OpenGL::VBO&	getVBO() const { return *_vbo; }
		inline OpenGL::VAO&	getVAO() const { return *_vao; }
		inline void*		getMappedMemory() const { return _mappedPtr; }
		std::int32_t 		getFreeSize() const { return maximumSize - currentSize; }
		void				copyData(SpriteVertex (&array)[6]);
	};

	class SpriteBatchManager : public Core::IManager
	{
		MANAGER(SpriteBatchManager);

		friend class Renderer;

		Memory::IMemoryBlock&		_memory;
		Utils::List<SpriteBuffer*>	_buffers;

		union BatchIndex
		{
			explicit BatchIndex(std::uint16_t camDepth, std::uint16_t matIndex)
					: cameraDepth(camDepth), materialID(matIndex)
			{ }

			std::uint32_t key;

			struct
			{
				std::uint16_t cameraDepth;
				std::uint16_t materialID;
			};

			const bool operator <(const BatchIndex& bi) const
			{
				return key < bi.key;
			}
		};

		Utils::List<SpriteBatch>			_batches;
		std::map<std::uint32_t, std::uint32_t> _batchMap;

		std::uint32_t _currentBuffer;

	public:
		explicit SpriteBatchManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~SpriteBatchManager() noexcept;

		SpriteBatchManager(SpriteBatchManager&&) = delete;
		SpriteBatchManager(const SpriteBatchManager&) = delete;
		SpriteBatchManager& operator=(SpriteBatchManager&&) = delete;
		SpriteBatchManager& operator=(const SpriteBatchManager&) = delete;

		bool initialize();
		void clear();
		auto createNewBuffer(std::uint32_t size) -> SpriteBuffer&;

		SpriteBatch& getSpriteBatch(Utils::Handle<Core::Material> material, Camera* camera,
									std::int32_t minimalSize = SpriteBuffer::DEFAULT_BUFFER_SIZE);

		inline auto getBuffers() -> Utils::List<SpriteBuffer*>& { return _buffers; }
	//	inline auto getCurrentBuffer() -> SpriteBuffer& { return *_buffers[_currentBuffer]; }
	};
}

#endif //YAGE_SPRITEBATCHMANAGER_H
