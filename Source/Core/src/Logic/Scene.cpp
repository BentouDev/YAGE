//
// Created by mrjaqbq on 17.04.16.
//

#include "Core/Logic/Scene.h"
#include "Core/Engine.h"
#include "Core/Logic/RenderingSystem.h"

namespace Logic
{
	Scene::Scene(Core::Engine& engine, Memory::IMemoryBlock& memory, const char* name)
		: _memory(memory), Name(name), engine(engine)
	{
		defaultCamera = YAGE_CREATE_NEW(_memory, Gfx::Camera)();

		defaultCamera->FOV = 3.14f / 4.0f;
		defaultCamera->nearCulling = 0.01f;
		defaultCamera->farCulling = 100.0f;
		defaultCamera->position = glm::vec3(4,3,3); // Camera is at (4,3,3), in World Space
		defaultCamera->forward = glm::vec3(-4,-3,-3), // and looks at the origin
		defaultCamera->up = glm::vec3(0,1,0);  // Head is up (set to 0,-1,0 to look upside-down)
	}

	Scene::~Scene()
	{
		Memory::Delete(_memory, defaultCamera);
	}

	void Scene::setWorld(World *newWorld)
	{
		if(world != nullptr)
		{
			// Remove all entities..
			// Load scene once again
		}

		world = newWorld;
	}

	void Scene::Update(const Core::GameTime& time)
	{

	}

	void Scene::Draw(const Core::GameTime& time, Gfx::Renderer& renderer)
	{
		// TODO: in update or draw?
		// go through each of components
		// scheme <- mesh -> component
		// so using that we can access batch for scheme & material pair

		// this can be divided into threads
	//	Rendering->update(time, renderer, defaultCamera, defaultViewport);

		// what main classes do we have?
		// submesh, material, light, postprocess
		// should postprocess be related to scene?
		// component data is not relevant
		// they all pass what is necessary to material, submesh and uniform data anyway
		// what is important?
		// who knows which data should be collected?
		// shaders know their data
		// meshes know theirs also
		// gather meshes by pass id
		// gather lights
	}

	void Scene::Start()
	{

	}

	void Scene::End()
	{

	}
}
