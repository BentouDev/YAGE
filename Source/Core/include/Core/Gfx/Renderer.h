//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Core/Gfx/OpenGl/OpenGLBase.h"
#include "Core/Gfx/CommandQueue.h"
#include "Rectangle.h"

#include <Utils/BorrowedPtr.h>
#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <Utils/Container.h>
#include <Utils/Color.hpp>

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
	class Material;
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
		Core::Material* material;
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

		// Will add quad to buffer
		// And use default 2D shader for this
	//	void drawRect(Rectangle rect, Utils::Color color);

		// Will draw quad, but in 3D!
		// With proper rotation and shit
	//	void drawQuad(Rectangle rect, Utils::Color color, glm::mat4);

	//	void drawLine(glm::vec3 start, glm::vec3 end, Utils::Color color);

		// {
		// SpriteDrawCall
		// Rect - vertex data
		// Texcoord* - two 2D vectors
		// Matrix* - where the hell would I put that...
		// Color* - can go to vertex data too
		// Material* - ???, batching probably!
		// }

		// void drawRect(Rectangle rect, Utils::Color color, Texture* texture);
		// void drawRect(Rectangle rect, Utils::Color color, Material);
		// void drawRect(Rectangle rect, Material);

		// Text rendering
		// basically
		// - methods for length and height calculation
		// - methods for length based offset calculation
		// - methods for getting begin/end for whole region
		// - methods for building VBO for those
	};
}

#endif //GAME_RENDERER_H
