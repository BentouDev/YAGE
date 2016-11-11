//
// Created by bentoo on 11/2/16.
//

#ifndef GAME_MATERIALBATCH_H
#define GAME_MATERIALBATCH_H

#include <Utils/DefaultTrait.h>
#include <Utils/Handle.h>
#include <Utils/List.h>

#include "../Resources/Mesh/MeshScheme.h"
#include "OpenGl/OpenGLBuffers.h"

namespace Core
{
	class Mesh;
	class Material;
}

namespace Gfx
{
	class StaticBuffer;

	struct MaterialBatchMeshReference;

	class MaterialBatch
	{
		friend class BatchManager;

	public:
		using handle_t = Utils::Handle<MaterialBatch>;

		handle_t Handle;

	protected:
		Utils::Handle<Core::Material>	_material;
		Resources::MeshSchemeId			_schemeId;

	public:
	//	bool hasSize(uint32_t vertexCount, uint32_t indexCount) const;
	//	bool hasMaterial(const Core::Material& material) const;
	//	bool hasMaterial(Utils::Handle<Core::Material> material) const;

	//	bool hasCombination(const Core::Mesh& mesh, const Core::Material& material) const;
	//	bool hasCombination(Utils::Handle<Core::Mesh> mesh, Utils::Handle<Core::Material> material) const;

	//	MaterialBatchMeshReference allocateMesh(const Core::Mesh& mesh);

		virtual auto swap(MaterialBatch& other) noexcept -> void
		{ }

		virtual auto cleanUp() noexcept -> void
		{ }
	};

	class MaterialBatchTrait : public Utils::DefaultTrait<MaterialBatch>
	{ };

	/*struct MaterialBatchMeshReference
	{
		MaterialBatch::handle_t BatchHandle;
		OpenGL::VAO* 			VAO;
		GLuint					BaseVertex;
		GLuint					IndexType;
		GLuint					ElementCount;
		GLuint					ShaderProgram;
	};*/
}

#endif //GAME_MATERIALBATCH_H
