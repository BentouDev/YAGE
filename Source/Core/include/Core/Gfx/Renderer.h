//
// Created by MrJaqbq on 2016-03-26.
//

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Core/Gfx/OpenGl/OpenGLBase.h"
#include "Core/Gfx/CommandQueue.h"
#include "Core/IManager.h"
#include "Rectangle.h"

#include <Utils/BorrowedPtr.h>
#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <Utils/Container.h>
#include <Utils/Color.hpp>
#include <Utils/Handle.h>

#include <map>

namespace Resources
{
	class Font;
}

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
	class Window;
}

namespace Gfx
{
	class BaseApi;
	class SpriteBatch;
	class SpriteBuffer;
	class SpriteBatchManager;

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

	class Renderer : public Core::IManager
	{
		MANAGER(Renderer);

	public:
		using queue_t = CommandQueue<RenderKey, RenderData, Logic::RenderingComponent>;

	protected:
		SpriteBatchManager*	_spriteBatchManager;

		queue_t _queue;

		GLuint lastIBO;
		GLuint lastVBO;
		GLuint lastVAO;
		GLuint lastProgram;

		GLuint cameraProjectionUniformLocation;
		GLuint cameraViewUniformLocation;
		GLuint cameraModelUniformLocation;

		Core::Material*	 _debugMaterial;
		Core::Material*	 _debugFontMaterial;
		Resources::Font* _debugFont;
		Gfx::Camera*	 _debug2DCamera;
		Gfx::Camera*	 _debug3DCamera;

		bool loadDebugAssets();
		bool createDebugCameras();

		bool initialize();

		void drawSpriteBatch(const SpriteBatch& buffer);

	public:
		Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~Renderer();

		inline queue_t& getQueue()
		{ return _queue; }

		bool registerWindow(const Core::Window*);

		void draw();
		void drawCall(RenderData&);

		Gfx::Camera&     getDebug3DCamera();
		Gfx::Camera&     getDebug2DCamera();
		Core::Material&  getDebugMaterial();
		Core::Material&  getDebugFontMaterial();
		Resources::Font& getDebugFont();

		Gfx::Camera& createCamera();
		SpriteBatch& getSpriteBatch(Utils::Handle<Core::Material> material, Camera* camera, std::int32_t minimalSize = -1);

		void drawSpriteBatches();

		// Will draw quad, but in 3D!
		// With proper rotation and shit
	//	void drawQuad(Rectangle rect, Utils::Color color, glm::mat4);

	//	void drawLine(glm::vec3 start, glm::vec3 end, Utils::Color color);

		// SpriteDrawCall
		// {
		//     Rect - vertex data
		//     Texcoord* - two 2D vectors
		//     Color* - can go to vertex data too
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
