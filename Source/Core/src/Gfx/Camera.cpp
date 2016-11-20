//
// Created by bentoo on 06.11.16.
//

#include "Core/Gfx/Camera.h"

namespace Gfx
{
	Camera::Camera()
	{

	}

	Camera::~Camera()
	{

	}

	const GLfloat* Camera::projectionPtr()
	{
		return &projectionMatrix[0][0];
	}

	const GLfloat* Camera::viewPtr()
	{
		return &viewMatrix[0][0];
	}

	void Camera::recalculate(float aspect)
	{
		viewMatrix			= glm::lookAt(position, position + forward, up);
		projectionMatrix	= glm::perspective(FOV, aspect, nearCulling, farCulling);
	}
}
