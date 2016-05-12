//
// Created by mrjaqbq on 17.04.16.
//

#ifndef GAME_MESHFACTORY_H
#define GAME_MESHFACTORY_H

#include <Resources/Mesh.h>
#include <fstream>

namespace Core
{
	class Engine;
}

namespace Resources
{
	class MeshParser;

	namespace MeshFactory
	{
		auto CreateMeshFromObj(std::string path, Core::Engine& engine) -> Core::MeshResource::Handle;

		auto LoadMeshFromObj(std::string path, Core::MeshResource& mesh) -> void;
	}

	template <typename T>
	class ObjParser
	{
		// some temporary data ?
	protected:
		std::map<std::string, bool(*)(std::string, T&)> LookupMap;

	public:
		auto ProcessLine(std::string line, T& obj) -> void
		{
			if(line.empty() || line[0] == '#')
				return;

			std::string token = line.substr(0, line.find(' '));

			if(!token.empty() && LookupMap.find(token) != LookupMap.end())
			{
				LookupMap[token](line, obj);
			}
		}
	};

	class MeshObjParser : public ObjParser<Core::MeshResource>
	{
	public:
		MeshObjParser();
	protected:
		static auto parseObject(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseVertexPos(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseTexcoord(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseNormal(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseFace(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseMaterial(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseMaterialUse(std::string line, Core::MeshResource& mesh) -> bool;

		static auto parseSmooth(std::string line, Core::MeshResource& mesh) -> bool;
	};
}


#endif //GAME_MESHFACTORY_H
