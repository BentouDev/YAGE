//
// Created by bentoo on 19.05.17.
//

#include "Core/Resources/Mesh/ModelLoader.h"
#include "Core/Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Resources
{
	ModelLoader::ModelLoader(Core::Engine &engine, Memory::IMemoryBlock &memory)
	: _engine(engine)
	, _memory(memory)
	, filename(memory)
	{

	}

	ModelLoader::~ModelLoader()
	{

	}

	ModelLoader& ModelLoader::loadFromFile(const char *path)
	{
		filename << path;
		return *this;
	}

	bool ModelLoader::buildRecurrent(const aiNode *node)
	{
		return false;
	}

	void ModelLoader::buildFromScene(const aiScene *scene, const char* meshName)
	{
		// postpone! MeshBuilder builder(_memory, _manager);

		// builder.withIndexType<std::uint32_t>();

		buildRecurrent(scene->mRootNode);

		// return builder.build(meshName);
	}

	void ModelLoader::build(const char* modelName)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile (
			filename.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType
		);

		if (!scene)
		{
			const char* errMessage = importer.GetErrorString();
			Core::Logger::get()->error (
					"MeshLoader : Unable to load mesh!\t\nfile : '{}',\nreason : {}",
					filename.c_str(), errMessage
			);
		}
		else
		{
			buildFromScene(scene, modelName);
		}
	}
}
