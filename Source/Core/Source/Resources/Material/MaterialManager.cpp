//
// Created by bentoo on 10/28/16.
//

#include "MaterialManager.h"
#include "../../Engine.h"
#include "../../Logger.h"

namespace Resources
{
	MaterialManager::MaterialManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _materialContainer(_memory)
	{

	}

	MaterialManager::~MaterialManager() noexcept
	{
		disposeAll();
	}

	void MaterialManager::disposeAll()
	{
		_engine.Logger->Default->info("Releasing all meshes...");

		for(Core::Material& material : _materialContainer)
		{
			disposeMaterial(material);
		}

		_materialContainer.clear();

		_engine.Logger->Default->info("Released all meshes");
	}

	Core::Material* MaterialManager::tryGetMaterial(handle_t handle)
	{
		Core::Material* ptr = nullptr;
		if(_materialContainer.contains(handle))
		{
			ptr = &getMaterial(handle);
		}
		return ptr;
	}

	void MaterialManager::disposeMaterial(Core::Material &material)
	{
		disposeMaterial(material.Handle);
	}

	void MaterialManager::disposeMaterial(handle_t handle)
	{
		// TODO: ADDITIONAL THINGS, LIKE STATIC BUFFER INVALIDATION

		_materialContainer.remove(handle);
	}

	void MaterialManager::uploadToBuffer(handle_t handle)
	{
		Core::Material* material = tryGetMaterial(handle);

		if(material == nullptr)
			return;


	}
}
