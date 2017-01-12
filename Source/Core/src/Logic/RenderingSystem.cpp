//
// Created by bentoo on 10/14/16.
//

#include <Core/Gfx/Camera.h>
#include <Core/Gfx/Viewport.h>
#include <Core/Logic/Scene.h>
#include "Core/Logic/RenderingSystem.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Resources/Mesh/Mesh.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Material/Material.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Logic
{
	RenderingSystem::RenderingSystem(Core::Engine &engine, Memory::IMemoryBlock& memory, World& world)
		: System(memory), _engine(engine), _memory(memory), _world(world), _dirtyComponents(memory), _sceneEntities(memory)
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
				OpenGL::checkError();

				if(location == -1)
				{
					// todo: Report error: there is no such attribute or its reserved for driver
					Core::Logger::get()->error (
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
		/*for(RenderingComponent::handle_t handle : _dirtyComponents)
		{
			// At this moment we don't know what was changed
			// Material list should stay untouched
			// Mesh and material pair data has to be reloaded

			RenderingComponent& comp = _components.get(handle);
			//if(comp._cachedSubmeshInfo.size() > 0)
			{
				for (auto& h : comp._cachedSubmeshInfo)
				{
				//	_engine.BatchManager.get().invalidateBatch(h.BatchHandle);
				}

				comp._cachedSubmeshInfo.clear();
			}//

			refreshDirtyComponent(comp);
		}

		_dirtyComponents.clear();*/
	}

	void RenderingSystem::refreshDirtyComponent(RenderingComponent& component)
	{
		Core::Mesh* mesh = _engine.MeshManager.get().tryGetMesh(component.getMesh());
		if(mesh != nullptr)
		{
			/*for (Core::Submesh& submesh : mesh->getSubmeshes())
			{
				//	const Core::Material& material = _engine.MaterialManager.get().getMaterial(matHandle);
				//	comp._batchHandles.add(_engine.BatchManager.get().allocateMesh(mesh, material));
			}*/

			createVAO(component);
		}
		else
		{
			// theres no mesh!
			// do swap if container supports
			component._cachedSubmeshInfo.clear();
			component._isVisible = false;
		}

		component._isDirty = false;
	}

	void RenderingSystem::update(const Core::GameTime& time)
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

		// Group entities by their camera
		// That will mean that this generic list that we are providing is worth shit
		// OnAddEntity or something will be overriden
		// And then some struct like CamBatch { Cam; List<Entity> } will be introduced

		for(SceneInfo& info : _sceneEntities)
		{
			for(Entity::handle_t handle : info.entities)
			{
				Gfx::Renderer::queue_t& queue = _engine.Renderer->getQueue();
				queue.setCamera(info.scene->defaultCamera);
				queue.setRenderTarget(info.scene->defaultViewport);

				// TODO: we are using handle to get entity, and World internaly extracts handle to pass it to manager, so getting this instance is useless
				Entity& entity = _world.getEntity(handle);
				RenderingComponent& comp = entity.getComponent<RenderingComponent>();

				// TODO: introduce activation to container and exclude inactive ones straight ahead
				// But that wont help, will it?
				if(!comp.isVisible())
					continue;

				// TODO: this cannot be called here, move it away!
				if(comp.isDirty())
					refreshDirtyComponent(comp);

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
					packet.material		 = &_engine.MaterialManager->getMaterial(comp._materials[i]);

					// VAO has only mesh scheme!
					packet.VAO = *info.VAO;
					packet.VBO = buffer.getVBO();
					packet.IBO = buffer.getIBO();
				}
			}
		}
	}

	void RenderingSystem::addEntity(ISystem::entity_handle_t handle)
	{
		Entity&	entity	= _world.getEntity(handle);
		bool	added	= false;

		for(SceneInfo& info : _sceneEntities)
		{
			if(info.scene == &entity.getScene())
			{
				added = true;
				info.entities.add(handle);
			}
		}

		if(!added)
		{
			_sceneEntities.emplace(&entity.getScene(), _memory, handle);
		}
	}
}
