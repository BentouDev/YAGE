//
// Created by bentoo on 07.11.16.
//

#ifndef GAME_RENDERINGCOMPONENT_H
#define GAME_RENDERINGCOMPONENT_H

#include <cstdint>

#include "Core/Logic/Component.h"
#include "Core/Gfx/OpenGl/OpenGLBuffers.h"

namespace Gfx
{
	class Renderer;
	class MaterialBatch;
}

namespace Core
{
	class Mesh;
	class Material;
}

namespace Logic
{
	class RenderingSystem;

	struct SubmeshInfo
	{
		SubmeshInfo() : ShaderProgram(0), VAO(nullptr)
		{ }

		GLuint ShaderProgram;
		OpenGL::VAO* VAO;
	};

	class RenderingComponent : public Component
	{
	public:
		friend class Gfx::Renderer;
		friend class RenderingSystem;

		using handle_t	= Utils::Handle<RenderingComponent>;

	private:
		bool _isVisible;
		bool _isDirty;

		Utils::Handle<Core::Mesh>					_mesh;
		Utils::List<Utils::Handle<Core::Material>>	_materials;
		Utils::List<SubmeshInfo>					_cachedSubmeshInfo;

	public:
		inline explicit RenderingComponent(Memory::IMemoryBlock& memory)
				: _isVisible(true), _isDirty(false),
				  _mesh(), _materials(), _cachedSubmeshInfo() // #NewAlloc
		{
			setDirty();
		}

		RenderingComponent(const RenderingComponent&) = delete;
		RenderingComponent& operator=(const RenderingComponent&) = delete;
		RenderingComponent& operator=(RenderingComponent&&) = delete;

		RenderingComponent(RenderingComponent&& other)
			: _isVisible(other._isVisible), _isDirty(false),
			  _mesh(std::move(other._mesh)),
			  _materials(std::move(other._materials)),
			  _cachedSubmeshInfo(std::move(other._cachedSubmeshInfo))
		{

		}

		virtual ~RenderingComponent();

		void setDirty();

		inline bool isDirty() const
		{ return _isDirty; }

		inline bool isVisible() const
		{ return _isVisible; }

		inline void hide()
		{ setVisibility(false); }

		inline void show()
		{ setVisibility(true); }

		inline void setVisibility(bool visible)
		{
			if(_isVisible != visible)
			{
				_isVisible = visible;
				setDirty();
			}
		}

		void setMaterial(Utils::Handle<Core::Material> material, std::uint32_t index);

		inline Utils::Handle<Core::Material> getMaterial(std::uint32_t index) const
		{
			// TODO: PROPER ERROR HANDLING
			assert(index < _cachedSubmeshInfo.size() && "Material index out of bounds");
			return _materials[index];
		}

		inline void setMesh(Utils::Handle<Core::Mesh> handle)
		{ _mesh = handle; }

		inline Utils::Handle<Core::Mesh> getMesh() const
		{ return _mesh; }

		inline const Utils::List<Utils::Handle<Core::Material>>& getMaterials() const
		{ return _materials; }
	};
}

#endif //GAME_RENDERINGCOMPONENT_H
