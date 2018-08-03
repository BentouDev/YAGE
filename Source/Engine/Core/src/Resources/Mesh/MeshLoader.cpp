//
// Created by Bentoo on 2017-01-02.
//

#include "Core/Logger.h"
#include "Core/Resources/Mesh/MeshBuilder.h"
#include "Core/Resources/Mesh/MeshLoader.h"
#include "Core/Resources/Mesh/MeshManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Resources
{
    MeshLoader::MeshLoader(Memory::IMemoryBlock& memory, MeshManager& manager)
        : _memory(memory), _manager(manager)
    {

    }

    MeshLoader::~MeshLoader()
    {

    }

    MeshLoader& MeshLoader::fromPath(const char *path)
    {
        filePath = path;
        return *this;
    }

    Core::Mesh::handle_t MeshLoader::buildFromScene(const aiScene *scene, const char* meshName) const
    {
        MeshBuilder builder(_memory, _manager);

        builder.withIndexType<std::uint32_t>();

        for(auto i = 0; i < scene->mNumMeshes; i++)
        {

        }

        return builder.build(meshName);
    }

    Core::Mesh::handle_t MeshLoader::build(const char* meshName) const
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile (
            filePath,
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType
        );

        if(!scene)
        {
            const char* errMessage = importer.GetErrorString();
            Core::Logger::error (
                "MeshLoader : Unable to load mesh!\t\nfile : '{}',\nreason : {}",
                filePath, errMessage
            );

            return Core::Mesh::handle_t::invalid();
        }
        else
        {
            return buildFromScene(scene, meshName);
        }
    }
}
