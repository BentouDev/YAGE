//
// Created by MrJaqbq on 2016-02-10.
//

#include "Mesh.h"

namespace Core
{
	std::size_t Mesh::getVertexCount() const
	{
		std::size_t result = 0;

		if(_data != nullptr && _data->getPropertiesData().size() > 0)
		{
			result = _data->getPropertiesData()[0].count();
		}

		return result;
	}

	std::size_t Mesh::getIndiceCount() const
	{
		std::size_t result = 0;
		for(auto& submesh : _submeshes)
		{
			result += submesh.getIndiceCount();
		}

		return result;
	}
}
