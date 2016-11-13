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

namespace Gfx
{
	class Camera
	{
		glm::mat4x4 _viewMatrix;
		glm::mat4x4 _projectionMatrix;

		float _FOV = 60;
		float _nearCulling = 0.01f;
		float _farCulling = 100.f;

	public:
		explicit Camera();
		virtual ~Camera();

		YAGE_PROPERTY_NAMED(_FOV, FOV);
		YAGE_PROPERTY_NAMED(_nearCulling, NearCulling);
		YAGE_PROPERTY_NAMED(_farCulling, FarCulling);
	};
}

#endif //GAME_CAMERA_H
