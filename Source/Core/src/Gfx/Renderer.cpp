//
// Created by bentoo on 10/19/16.
//

#include <glm/gtc/matrix_transform.hpp>
#include <Utils/MemorySizes.h>

#include "Core/Gfx/Renderer.h"
#include "Core/Gfx/Camera.h"
#include "Core/Gfx/RenderTarget.h"
#include "Core/Gfx/Sprite.h"
#include "Core/Gfx/SpriteBatch.h"
#include "Core/Gfx/SpriteBatchManager.h"

#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/Resources/Mesh/Mesh.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Material/Material.h"
#include "Core/Resources/Material/DefaultMaterial.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Resources/Shader/Shader.h"
#include "Core/Resources/Shader/ShaderBuilder.h"
#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Resources/Font/Font.h"
#include "Core/Resources/Font/FontLoader.h"
#include "Core/Resources/Font/FontManager.h"
#include "Core/Resources/Texture/TextureLoader.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Logic/RenderingSystem.h"

namespace Gfx
{
	Renderer::Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: IManager(engine, memory),
		  _spriteBatchManager(_engine.CreateManager<SpriteBatchManager>(Memory::KB(100))),
		  _queue(_memory, *this),
		  _debugMaterial(nullptr),
		  _debugFontMaterial(nullptr), _debugFont(nullptr),
		  _debug2DCamera(nullptr), _debug3DCamera(nullptr),
		  lastIBO(0), lastVBO(0), lastVAO(0), lastProgram(0),
		  cameraProjectionUniformLocation(1), cameraViewUniformLocation(2)
	{ }

	Renderer::~Renderer()
	{
		Memory::Delete(_engine.MemoryModule->masterBlock(), _spriteBatchManager);
		Memory::Delete(_memory, _debug2DCamera);
		Memory::Delete(_memory, _debug3DCamera);
	}

	bool Renderer::loadDebugAssets()
	{
		if (_debugMaterial == nullptr)
		{
			auto  matHandle = _engine.MaterialManager->createMaterial();
			auto* material  = _engine.MaterialManager->tryGetMaterial(matHandle);

			Resources::ShaderBuilder builder(_engine, _memory);
			auto debugShader = builder
					.withVertexFromSource(Resources::DEBUG_MATERIAL_VERTEX_SOURCE)
					.withFragmentFromSource(Resources::DEBUG_MATERIAL_FRAGMENT_SOURCE)
					.withAttributeLocation(0, "position")
					.withAttributeLocation(1, "texcoord")
					.withAttributeLocation(2, "color")
					.debugBuild("Renderer::DebugMaterial");

			if (material == nullptr || debugShader == ShaderProgram::handle_t::invalid())
				return false;

			_debugMaterial = material;
			_debugMaterial->setShaderProgram(debugShader);
		}

		if (_debugFontMaterial == nullptr)
		{
			auto  matHandle = _engine.MaterialManager->createMaterial();
			auto* material  = _engine.MaterialManager->tryGetMaterial(matHandle);

			Resources::ShaderBuilder builder(_engine, _memory);
			auto fontShader = builder
					.withVertexFromFile("../Data/Shaders/FontVertex.glsl")
					.withFragmentFromFile("../Data/Shaders/FontFragment.glsl")
					.withAttributeLocation(0, "position")
					.withAttributeLocation(1, "texcoord")
					.withAttributeLocation(2, "color")
					.debugBuild("Renderer::FontShader");

			if (material == nullptr || fontShader == ShaderProgram::handle_t::invalid())
				return false;

			_debugFontMaterial = material;
			_debugFontMaterial->setShaderProgram(fontShader);
		}

		if (_debugFont == nullptr)
		{
			Resources::TextureLoader textureLoader(_engine.TextureManager.get());
			textureLoader
					.setParameter(gl::TEXTURE_MAG_FILTER, gl::LINEAR)
					.setParameter(gl::TEXTURE_MIN_FILTER, gl::LINEAR)
					.setParameter(gl::TEXTURE_WRAP_S, gl::REPEAT)
					.setParameter(gl::TEXTURE_WRAP_T, gl::REPEAT);

			Resources::FontLoader fontLoader(_engine.FontManager.get());
			auto font_handle = fontLoader
					.withTextureLoader(textureLoader)
					.loadFromFile("../Data/Fonts/fantasque.fnt")
					.build();

			auto* font = _engine.FontManager->tryGetFont(font_handle);

			if (font == nullptr || font_handle == Resources::Font::handle_t::invalid())
				return false;

			_debugFont = font;

			_debugFontMaterial->addUniform<Resources::Texture *>("diffuseTex", _engine.ShaderManager.get());
			_debugFontMaterial->setUniform (
				"diffuseTex",
				_engine.TextureManager->tryGetTexture (
					_debugFont->getDefaultTexture()
				)
			);
		}

		return _debugMaterial != nullptr
			&& _debugFontMaterial != nullptr
			&& _debugFont != nullptr;
	}

	bool Renderer::createDebugCameras()
	{
		if (_debug2DCamera == nullptr)
		{
			_debug2DCamera = &createCamera();
		}

		if (_debug3DCamera == nullptr)
		{
			_debug3DCamera = &createCamera();
		}

		return _debug2DCamera != nullptr
		    && _debug3DCamera != nullptr;
	}

	bool Renderer::initialize()
	{
		bool result = true;
		result &= _spriteBatchManager->initialize();
		result &= loadDebugAssets();
		result &= createDebugCameras();

		return result;
	}

	bool Renderer::registerWindow(const Core::Window* windowPtr)
	{
		bool result = true;
		result &= OpenGL::registerWindow(*windowPtr);
		result &= initialize();

		return result;
	}

	Gfx::Camera& Renderer::createCamera()
	{
		Gfx::Camera* camPtr = YAGE_CREATE_NEW(_memory, Camera)();
		return *camPtr;
	}

	SpriteBatch& Renderer::getSpriteBatch(Utils::Handle<Core::Material> material, Camera* camera, int32_t minimalSize)
	{
		return _spriteBatchManager->getSpriteBatch(material, camera, minimalSize);
	}

	Gfx::Camera& Renderer::getDebug3DCamera()
	{
		return *_debug3DCamera;
	}

	Gfx::Camera& Renderer::getDebug2DCamera()
	{
		return *_debug2DCamera;
	}

	Core::Material& Renderer::getDebugMaterial()
	{
		return *_debugMaterial;
	}

	Core::Material& Renderer::getDebugFontMaterial()
	{
		return *_debugFontMaterial;
	}

	Resources::Font& Renderer::getDebugFont()
	{
		return *_debugFont;
	}

	void Renderer::drawSpriteBatch(const SpriteBatch& batch)
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, batch.getBuffer().getVBO());
		OpenGL::checkError();

		gl::DrawArrays(gl::TRIANGLES, batch.getOffset(), batch.getSize());
		OpenGL::checkError();

		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
		OpenGL::checkError();
	}

	void Renderer::drawSpriteBatches()
	{
		// For each render target
		// For each camera
		// For each material

		for(auto* buffer : _spriteBatchManager->_buffers)
		{
			buffer->unmapMemory();
		}

		for(auto itr : _spriteBatchManager->_batchMap)
		{
			const glm::mat4	modelMatrix	= glm::mat4(1.0f);
			const auto&		batch		= _spriteBatchManager->_batches[itr.second];
			Gfx::Camera*	camera		= batch.getCamera();

			if(camera == nullptr
		    || camera->getRenderTarget() == nullptr
		    || batch.getMaterial() == nullptr)
				continue;

			// Bind cameras render target
			camera->getRenderTarget()->Bind();

			gl::BlendFunc(batch.blendSfactor, batch.blendDfactor);

			// Bind material uniforms
			auto program_handle = batch.getMaterial()->getShaderProgram();
			auto& program = _engine.ShaderManager->get(program_handle);
			gl::UseProgram(program);
			OpenGL::checkError();

			batch.getMaterial()->bindUniforms();
			OpenGL::checkError();

			// Recalculate camera
			camera->recalculate();

			// Bind camera uniforms
			gl::UniformMatrix4fv(1, 1, gl::FALSE_, camera->projectionPtr());
			gl::UniformMatrix4fv(2, 1, gl::FALSE_, camera->viewPtr());
			gl::UniformMatrix4fv(3, 1, gl::FALSE_, &modelMatrix[0][0]);
			OpenGL::checkError();

			gl::BindBuffer(gl::ARRAY_BUFFER, batch.getBuffer().getVBO());
			OpenGL::checkError();

			gl::BindVertexArray(batch.getBuffer().getVAO());

			gl::DrawArrays(gl::TRIANGLES, batch.getOffset(), batch.getSize());
			OpenGL::checkError();

			gl::BindVertexArray(0);
			OpenGL::checkError();

			gl::BindBuffer(gl::ARRAY_BUFFER, 0);
			OpenGL::checkError();

			// drawSpriteBatch(batch);
		}

		gl::BindVertexArray(0);
		_spriteBatchManager->clear();
	}

	void Renderer::drawCall(RenderData& data)
	{
		if(lastProgram != data.ShaderProgram)
		{
			lastProgram = data.ShaderProgram;
			gl::UseProgram(data.ShaderProgram);
		}

		if(data.material != nullptr)
			data.material->bindUniforms();

		if(lastVAO != data.VAO)
		{
			lastVAO = data.VAO;
			gl::BindVertexArray(data.VAO);
		}

		if(lastVBO != data.VBO)
		{
			lastVBO = data.VBO;
			gl::BindBuffer(gl::ARRAY_BUFFER, data.VBO);
		}

		if(lastIBO != data.IBO)
		{
			lastIBO = data.IBO;
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, data.IBO);
		}

		gl::DrawElementsBaseVertex(gl::TRIANGLES, data.elementCount, data.indexType, 0, data.baseVertex);
	}

	void Renderer::draw()
	{
// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);

	//	gl::UniformMatrix4fv(1, 1, gl::FALSE_, &Projection[0][0]);
	//	gl::UniformMatrix4fv(2, 1, gl::FALSE_, &View[0][0]);
	//	gl::UniformMatrix4fv(3, 1, gl::FALSE_, &Model[0][0]);

		if(_queue._camera == nullptr || _queue._renderTarget == nullptr)
			return;

		//_queue.sort();
		//_queue.issueCommands();

		_queue._camera->recalculate(_queue._renderTarget->getAspect());

		gl::UniformMatrix4fv(1, 1, gl::FALSE_, _queue._camera->projectionPtr());
		gl::UniformMatrix4fv(2, 1, gl::FALSE_, _queue._camera->viewPtr());
		gl::UniformMatrix4fv(3, 1, gl::FALSE_, &Model[0][0]);

		for(auto& data : _queue._data)
		{
			drawCall(data);
		}

		_queue._data.clear();
		_queue._keys.clear();

		/*const std::size_t bucketCount = _buckets.size();
		for(std::size_t i = 0; i < bucketCount; i++)
		{
			const Gfx::RenderPass& bucket = _buckets[i];
			const Utils::List<Gfx::RenderBatch>& batches = bucket.getBatches();
			const std::size_t batchCount = batches.size();

			//

			// do vbo upload data by scheme, as intended


			for(std::size_t j = 0; j < batchCount; j++)
			{
				const Gfx::RenderBatch& batch = batches[j];
				const Utils::List<Gfx::RenderBatch::MaterialInfo>& materials = batch.getMaterialBatches();
				const std::size_t materialCount = materials.size();

				// batch.getVAO().bind();
				// which binds vbo and ibo with apropriate strides

				for (int k = 0; k < materialCount; k++)
				{
					const Gfx::RenderBatch::MaterialInfo& material = materials[k];
					const std::size_t submeshCount = material.submeshes.size();

					// binds material data for all meshes as a buffer
					// bind shader ofc
					// but bind vao?

					for(std::size_t l = 0; l < submeshCount; l++)
					{
						const Gfx::RenderBatch::SubmeshInfo& mesh = material.submeshes[l];
						gl::DrawElementsBaseVertex(gl::TRIANGLES, mesh.count, mesh.indexType, 0, mesh.baseVertex);
					}
				}
			}
		}*/
	}
}