//
// Created by bentoo on 10/25/16.
//

#ifndef GAME_MESHSCHEMEMANAGER_H
#define GAME_MESHSCHEMEMANAGER_H

#include <cstdint>
#include <Utils/List.h>
#include "Mesh.h"

namespace Resources
{
	class MeshSchemeManager
	{
		Memory::IMemoryBlock& _memory;
		Utils::List<Core::MeshScheme> _registeredSchemes;

		MeshSchemeId registerScheme(const Core::MeshScheme& scheme);

	public:
		explicit MeshSchemeManager(Memory::IMemoryBlock& memory);

		MeshSchemeId getSchemeId(const Core::MeshScheme& scheme);
		Core::MeshScheme* tryGetMeshScheme(MeshSchemeId id);
		bool contains(const Core::MeshScheme& scheme) const;
	};
}

#endif //GAME_MESHSCHEMEMANAGER_H
