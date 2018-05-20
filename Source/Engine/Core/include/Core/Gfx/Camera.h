//
// Created by bentoo on 06.11.16.
//

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Utils/PropertyMacro.h>
#include "Core/Gfx/OpenGl/OpenGLBase.h"
#include "Core/Gfx/Rectangle.h"

namespace Gfx
{
	class RenderTarget;

	namespace CameraMode
	{
		enum Type
		{
			Perspective,
			Orthographic
		};
	}

	class Camera
	{
		friend class Renderer;

		RenderTarget* renderTarget;

		const GLfloat* projectionPtr() const;
		const GLfloat* viewPtr() const;

		void recalculate();

		bool  hasForcedAspect;
		float forcedAspect;

	public:
		explicit Camera();
		virtual ~Camera();

		void bindUniforms();

		void forceAspect(bool force, float value);
		void setRenderTarget(RenderTarget* target);
		void setRenderTarget(RenderTarget& target);
		auto getRenderTarget() -> RenderTarget*;
		auto getRenderTarget() const -> const RenderTarget*;

		CameraMode::Type		mode;
		bool 					enableScissors;
		Gfx::Rectangle<float>	scissors;
		glm::mat4x4 			viewMatrix;
		glm::mat4x4 			projectionMatrix;
		glm::vec3				position;
		glm::vec3				forward;
		glm::vec3				up;

		float					FOV = 60;
		float					nearCulling = 0.01f;
		float					farCulling = 100.f;
		std::uint16_t			sortIndex;
	};
}

#endif //GAME_CAMERA_H
