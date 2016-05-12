//
// Created by mrjaqbq on 17.04.16.
//

#include "MeshFactory.h"

namespace Resources
{
	namespace MeshFactory
	{
//		auto CreateMeshFromObj(std::string path, Core::Engine& engine) -> Core::MeshResource::Handle
//		{
			// todo: resolve error with handle wanting trait instead of actual type
//			Core::MeshResource::Handle handle;

//			handle = engine.MeshManager->Create();
//			LoadMeshFromObj(path, engine.MeshManager->Get(handle));

//			return handle;
//		}

		auto LoadMeshFromObj(std::string path, Core::MeshResource& mesh) -> void
		{
			MeshObjParser parser;

			std::ifstream file(path);

			std::string line;
			while(getline(file, line))
			{
				parser.ProcessLine(line, mesh);
			}
		}
	}

	MeshObjParser::MeshObjParser() : ObjParser()
	{
		LookupMap["o"] = &MeshObjParser::parseObject;
		LookupMap["v"] = &MeshObjParser::parseVertexPos;
		LookupMap["vt"] = &MeshObjParser::parseTexcoord;
		LookupMap["vn"] = &MeshObjParser::parseNormal;
		LookupMap["f"] = &MeshObjParser::parseFace;
		LookupMap["mtlib"] = &MeshObjParser::parseMaterial;
		LookupMap["usemtl"] = &MeshObjParser::parseMaterialUse;
		LookupMap["s"] = &MeshObjParser::parseSmooth;
	}

	auto MeshObjParser::parseObject(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseVertexPos(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseTexcoord(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseNormal(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseFace(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseMaterial(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseMaterialUse(std::string line, Core::MeshResource& mesh) -> bool
	{

	}

	auto MeshObjParser::parseSmooth(std::string line, Core::MeshResource& mesh) -> bool
	{

	}
}
