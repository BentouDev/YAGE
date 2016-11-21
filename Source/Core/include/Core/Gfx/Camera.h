//
// Created by bentoo on 06.11.16.
//

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Utils/PropertyMacro.h>
#include "Core/Gfx/OpenGl/OpenGLBase.h"

namespace Gfx
{
	class Camera
	{
		friend class Renderer;

		const GLfloat* projectionPtr();
		const GLfloat* viewPtr();

		void recalculate(float aspect);

	public:
		explicit Camera();
		virtual ~Camera();

		glm::mat4x4 viewMatrix;
		glm::mat4x4 projectionMatrix;
		glm::vec3	position;
		glm::vec3	forward;
		glm::vec3	up;

		float		FOV = 60;
		float		nearCulling = 0.01f;
		float		farCulling = 100.f;
	};
}

#endif //GAME_CAMERA_H
