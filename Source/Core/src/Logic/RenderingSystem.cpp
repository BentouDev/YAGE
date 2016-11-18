//
// Created by bentoo on 10/14/16.
//

#include "Core/Logic/RenderingSystem.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Resources/Mesh/Mesh.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Material/Material.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Logic
{
	RenderingSystem::RenderingSystem(Core::Engine &engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _components(memory), _dirtyComponents(memory)
	{

	}

	void RenderingSystem::setDirty(RenderingComponent &comp)
	{
		if(!comp._isDirty)
		{
			comp._isDirty = true;
			_dirtyComponents.add(comp.Handle);
		}
	}

	RenderingComponent::handle_t RenderingSystem::createNew()
	{
		RenderingComponent::handle_t handle = _components.create(*this, _memory);

		return handle;
	}

	void RenderingSystem::remove(RenderingComponent::handle_t handle)
	{
		// auto& cmp = _components.get(handle);

		// dereference mesh
		// dereference materials
		// dereference textures

		_components.remove(handle);
	}

	RenderingComponent& RenderingSystem::get(RenderingComponent::handle_t handle) const
	{
		return _components.get(handle);
	}

	void RenderingSystem::createVAO(RenderingComponent& component)
	{
		Core::Mesh* mesh = _engine.MeshManager.get().tryGetMesh(component.getMesh());

		if(mesh == nullptr)
			return;

		Core::MeshScheme* scheme = _engine.MeshManager.get().tryGetMeshScheme(mesh->getMeshSchemeId());

		if(scheme == nullptr)
			return;

		for(auto& info : component._cachedSubmeshInfo)
		{
			Memory::Delete(_memory, info.VAO);
		}

		component._cachedSubmeshInfo.resize(std::min(mesh->getSubmeshes().size(), component.getMaterials().size()));

		const std::size_t vertexSize = scheme->vertexSize();

		for(std::size_t i = 0; i < component._cachedSubmeshInfo.size(); i++)
		{
			SubmeshInfo&	submesh = component._cachedSubmeshInfo[i];
			OpenGL::VAO*	vao		= OpenGL::VAO::Create(_memory);
			GLuint			offset	= 0;

			gl::BindVertexArray(*vao);

			Core::Material& material = _engine.MaterialManager.get().getMaterial(component.getMaterials()[i]);
			Gfx::ShaderProgram& shader = _engine.ShaderManager.get().get(material.getShaderProgram());

			for(Core::MeshScheme::PropertyInfo& info : scheme->getPropertiesInfo())
			{
				GLint location = gl::GetAttribLocation(shader, info.Name);
				OpenGL::checkError(_engine.Logger.get());
				if(location == -1)
				{
					// todo: Report error: there is no such attribute or its reserved for driver
					_engine.Logger.get().Default->error (
						"createVAO : theres no attribute named '{}' or its driver reserved.", info.Name
					);
				}
				else
				{
					gl::EnableVertexAttribArray((GLuint)location);
					// stride is size od vertex, offset is size of previous components
					gl::VertexAttribPointer((GLuint)location, info.PropertyCount, OpenGL::toOpenGlType(info.Type),
											(GLboolean)info.Normalize, (GLsizei)vertexSize, reinterpret_cast<void*>(offset));

					offset += info.PropertySize * info.PropertyCount;
				}
			}

			submesh.VAO = vao;
			submesh.ShaderProgram = shader;

			gl::BindVertexArray(0);
		}
	}

	void RenderingSystem::refreshDirtyComponents()
	{
		for(RenderingComponent::handle_t handle : _dirtyComponents)
		{
			// At this moment we don't know what was changed
			// Material list should stay untouched
			// Mesh and material pair data has to be reloaded

			RenderingComponent& comp = _components.get(handle);
			/*if(comp._cachedSubmeshInfo.size() > 0)
			{
				for (auto& h : comp._cachedSubmeshInfo)
				{
				//	_engine.BatchManager.get().invalidateBatch(h.BatchHandle);
				}

				comp._cachedSubmeshInfo.clear();
			}*/

			Core::Mesh* mesh = _engine.MeshManager.get().tryGetMesh(comp.getMesh());
			if(mesh != nullptr)
			{
				/*for (Core::Submesh& submesh : mesh->getSubmeshes())
				{
					//	const Core::Material& material = _engine.MaterialManager.get().getMaterial(matHandle);
					//	comp._batchHandles.add(_engine.BatchManager.get().allocateMesh(mesh, material));
				}*/

				createVAO(comp);
			}
			else
			{
				// theres no mesh!
				// do swap if container supports
				comp._cachedSubmeshInfo.clear();
				comp._isVisible = false;
			}

			comp._isDirty = false;
		}

		_dirtyComponents.clear();
	}

	void RenderingSystem::update(const Core::GameTime&, Gfx::Renderer& renderer)
	{
		// recreate dirty batches
		// add new meshes to them or change their data

		refreshDirtyComponents();

		// CULLING WOULD BE DONE HERE?
		// construct Command data for queue
		// pass this queue to renderer

		// Created by renderer? User probably
		// So there should be something like pass or technique class
		// That will get rendering components (or other ones) by type/tag
		// From appropriate system (maybe on separate threads even)!
		// Then it will sort them in renderer
		// And submit calls to GPU
		Gfx::Renderer::queue_t& queue = renderer.getQueue();
		for(RenderingComponent& comp : _components)
		{
			if(!comp.isVisible())
				continue;

			// Get new packet
			// Set key information (this is only used for sorting, co no stress!)
			const Core::Mesh&					mesh		= _engine.MeshManager.get().getMesh(comp.getMesh());
			const Gfx::StaticBuffer&			buffer		= _engine.BufferManager.get().getBuffer(mesh.getBuffer());
			const Core::MeshScheme&				scheme		= _engine.MeshManager.get().getMeshScheme(mesh.getMeshSchemeId());
			const Utils::List<Core::Submesh>&	submeshes	= mesh.getSubmeshes();

			// TODO: Maybe cachce entire submesh data?
			for(std::size_t i = 0; i < comp._cachedSubmeshInfo.size(); i++)
			{
				const Core::Submesh& submesh = submeshes[i];
				const SubmeshInfo& info = comp._cachedSubmeshInfo[i];

				Gfx::RenderKey key;
				Gfx::RenderData& packet = queue.createCommands(key);

				// Set Data, like shader program, uniforms, vao etc.
				// No uniforms as for now
				packet.baseVertex	 = (GLuint) submesh.getBaseVertex();
				packet.elementCount	 = (GLuint) submesh.getIndiceCount();
				packet.indexType	 = scheme.getIndexType();
				packet.ShaderProgram = info.ShaderProgram;

				// VAO has only mesh scheme!
				packet.VAO = *info.VAO;
				packet.VBO = buffer.getVBO();
				packet.IBO = buffer.getIBO();
			}
		}
	}
}
