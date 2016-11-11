//
// Created by bentoo on 10/25/16.
//

#include "MeshSchemeManager.h"

namespace Resources
{
	MeshSchemeManager::MeshSchemeManager(Memory::IMemoryBlock& memory)
		: _memory(memory), _registeredSchemes(_memory)
	{

	}

	MeshSchemeId MeshSchemeManager::registerScheme(const Core::MeshScheme& scheme)
	{
		_registeredSchemes.add(scheme);
		return (MeshSchemeId) (_registeredSchemes.size() - 1);
	}

	bool MeshSchemeManager::contains(const Core::MeshScheme &scheme) const
	{
		for(Core::MeshScheme& registeredScheme : _registeredSchemes)
		{
			if(registeredScheme == scheme)
			{
				return true;
			}
		}

		return false;
	}

	MeshSchemeId MeshSchemeManager::getSchemeId(const Core::MeshScheme& scheme)
	{
		for(MeshSchemeId i = 0; i < _registeredSchemes.size(); i++)
		{
			if(_registeredSchemes[i] == scheme)
			{
				return i;
			}
		}

		return registerScheme(scheme);
	}

	Core::MeshScheme* MeshSchemeManager::tryGetMeshScheme(MeshSchemeId id)
	{
		if(id < _registeredSchemes.size())
		{
			return &_registeredSchemes[id];
		}

		return nullptr;
	}
}
