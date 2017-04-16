//
// Created by bentoo on 06.11.16.
//

#include "Core/Gfx/Camera.h"
#include "Core/Gfx/RenderTarget.h"

namespace Gfx
{
	Camera::Camera()
		: renderTarget(nullptr),
		  enableScissors(false), scissors(0,0,1,1),
		  position(0,0,0), forward(0,0,1), up(0,1,0)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::setRenderTarget(RenderTarget* target)
	{
		renderTarget = target;
	}

	void Camera::setRenderTarget(RenderTarget& target)
	{
		renderTarget = &target;
	}

	Gfx::RenderTarget* Camera::getRenderTarget()
	{
		return renderTarget;
	}

	const Gfx::RenderTarget* Camera::getRenderTarget() const
	{
		return renderTarget;
	}

	const GLfloat* Camera::projectionPtr() const
	{
		return &projectionMatrix[0][0];
	}

	const GLfloat* Camera::viewPtr() const
	{
		return &viewMatrix[0][0];
	}

	void Camera::recalculate(float aspect)
	{
		viewMatrix			= glm::lookAt(position, position + forward, up);
		projectionMatrix	= glm::perspective(FOV, aspect, nearCulling, farCulling);
	}

	void Camera::recalculate()
	{
		if(renderTarget == nullptr)
			return;

		viewMatrix			= glm::lookAt(position, position + forward, up);
		projectionMatrix	= glm::perspective(FOV, renderTarget->getAspect(), nearCulling, farCulling);
	}

	void Camera::bindUniforms()
	{

	}
}
