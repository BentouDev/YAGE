//
// Created by bentoo on 07.11.16.
//

#include "Core/Logic/RenderingComponent.h"

#include "Core/Engine.h"
#include "Core/Logic/RenderingSystem.h"
#include "Core/Resources/Mesh/Mesh.h"
#include "Core/Resources/Mesh/MeshManager.h"

namespace Logic
{
	void RenderingComponent::setMaterial(Utils::Handle<Core::Material> material, std::uint32_t index)
	{
		/*if (_mesh != Utils::Handle<Core::Mesh>::invalid())
		{
			Core::Mesh *mesh = _system._engine.MeshManager.get().tryGetMesh(_mesh);
			if (mesh != nullptr)
			{
				mesh->getSubmeshes();
			}
		}*/

		_materials.resize(index+1);
		_materials[index] = material;

		// TODO: protect from creating more materials than mesh supports,
		// or just set size when setting mesh and force list bounds in here!
		//	_debugMaterials.resize(index + 1);
		//	_debugMaterials[index] = material.Handle;
		setDirty();
	}

	void RenderingComponent::setDirty()
	{
	//	_system.setDirty(*this);
		_isDirty = true;
	}

	RenderingComponent::~RenderingComponent()
	{
		for(auto& info : _cachedSubmeshInfo)
		{
			assert(false && "FIXME : LEAK");
		//	Memory::Delete(_memory, info.VAO);
		}

		_cachedSubmeshInfo.clear();
		_materials.clear();
	}
}
