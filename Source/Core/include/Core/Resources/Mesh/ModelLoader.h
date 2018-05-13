//
// Created by bentoo on 19.05.17.
//

#ifndef YAGE_MODELLOADER_H
#define YAGE_MODELLOADER_H

#include <Utils/String.h>

namespace Core
{
	class Engine;
}

namespace Memory
{
	class IMemoryBlock;
}

// Assimp predeclare
struct aiScene;
struct aiNode;

namespace Resources
{
	class ModelLoader
	{
		Core::Engine& 			_engine;
		Memory::IMemoryBlock&	_memory;

		Utils::String 			filename;

		bool buildRecurrent(const aiNode* node);
		void buildFromScene(const aiScene* scene, const char* meshName);

	public:
				 ModelLoader(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~ModelLoader();

		ModelLoader& forWorld();
		ModelLoader& loadFromFile(const char* path);
		void build(const char* modelName = nullptr);
	};
}

#endif //YAGE_MODELLOADER_H
