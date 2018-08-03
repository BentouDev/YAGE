//
// Created by Bentoo on 2017-01-02.
//

#ifndef GAME_MESHLOADER_H
#define GAME_MESHLOADER_H

#include "Mesh.h"

// Assimp predeclare
struct aiScene;

namespace Resources
{
	class MeshManager;
	class MeshBuilder;

	class MeshLoader
	{
		Memory::IMemoryBlock&	_memory;
		MeshManager&			_manager;

		const char* filePath;

		Core::Mesh::handle_t buildFromScene(const aiScene* scene, const char* meshName) const;

	public:
		explicit MeshLoader(Memory::IMemoryBlock& memory, MeshManager& manager);
		virtual ~MeshLoader();

		MeshLoader& fromPath(const char* path);

		Core::Mesh::handle_t build(const char* meshName) const;
	};
}

#endif //GAME_MESHLOADER_H
