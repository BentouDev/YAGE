//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "../Context.h"
#include "RenderPass.h"
#include "CommandQueue.h"

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

	union RenderKey
	{

	};

	struct RenderData
	{
		GLuint VAO;
		GLuint ShaderProgram;
		GLuint baseVertex;
		GLenum indexType;
		GLuint elementCount;
	};

	class Renderer
	{
	public:
		using queue_t = CommandQueue<RenderKey, RenderData, Logic::RenderingComponent>;

	protected:
		GLuint lastVAO;
		GLuint lastProgram;

		Memory::IMemoryBlock& 	_memory;
		Utils::List<RenderPass>	_buckets;

		Core::Engine& _engine;
		Core::Context _context;

		queue_t _queue;

	public:
		Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory);

		inline queue_t& getQueue()
		{ return _queue; }

		void draw();
		void drawCall(RenderData&);
		void createDrawCallData(Logic::RenderingComponent&);

	};
}

#endif //GAME_RENDERER_H
