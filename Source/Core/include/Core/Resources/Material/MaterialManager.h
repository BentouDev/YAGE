//
// Created by bentoo on 10/28/16.
//

#ifndef GAME_MATERIALMANAGER_H
#define GAME_MATERIALMANAGER_H

#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>
#include "Material.h"
#include "Core/IManager.h"

namespace Core
{
	class Engine;
}

namespace Resources
{
	class MaterialManager : public Core::IManager
	{
		MANAGER(MaterialManager);

		using handle_t = Core::Material::handle_t;

		Utils::Container<Core::MaterialTrait> _materialContainer;

		void disposeMaterial(Core::Material& material);

	public:
		explicit MaterialManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~MaterialManager() noexcept;

		MaterialManager(const MaterialManager&) = delete;
		MaterialManager(MaterialManager&&) = delete;

		void disposeAll();
		void reloadAll();

		void disposeMaterial(handle_t material);

		Core::Material* tryGetMaterial(handle_t);

		void uploadToBuffer(handle_t handle);

		inline bool hasMesh(handle_t handle)
		{ return _materialContainer.contains(handle); }

		inline handle_t createMaterial()
		{ return _materialContainer.create(_memory); }

		inline Core::Material& getMaterial(handle_t handle)
		{ return _materialContainer.get(handle); }
	};
}

#endif //GAME_MATERIALMANAGER_H
