//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Core/Context.h"
#include "Core/Gfx/OpenGl/OpenGLBase.h"
#include "Core/Gfx/CommandQueue.h"

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
		GLuint VBO;
		GLuint IBO;
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
		Core::Engine& _engine;

		Memory::IMemoryBlock& 	_memory;

		queue_t _queue;

		GLuint lastIBO;
		GLuint lastVBO;
		GLuint lastVAO;
		GLuint lastProgram;

	public:
		Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory);

		inline queue_t& getQueue()
		{ return _queue; }

		void draw();
		void drawCall(RenderData&);

	};
}

#endif //GAME_RENDERER_H
