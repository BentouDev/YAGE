//
// Created by bentoo on 06.11.16.
//

#include <Platform/Graphics/RenderTarget.h>
#include "Core/Gfx/Camera.h"

namespace Gfx
{
    Camera::Camera()
        : renderTarget(nullptr),
          mode(CameraMode::Perspective),
          hasForcedAspect(false),
          forcedAspect(1),
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

    void Camera::forceAspect(bool force, float value)
    {
        hasForcedAspect = force;
        if (hasForcedAspect)
            forcedAspect = value;
    }

    void Camera::recalculate()
    {
        if (renderTarget == nullptr)
            return;

        float aspect = hasForcedAspect ? forcedAspect : renderTarget->getAspect();
        viewMatrix   = glm::lookAt(position, position + forward, up);

        switch (mode)
        {
            case CameraMode::Perspective:
                projectionMatrix = glm::perspective(FOV, aspect, nearCulling, farCulling);
                break;

            case CameraMode::Orthographic:
                auto& rect = renderTarget->getUnitRect();
                projectionMatrix = glm::ortho(-rect.getLeft(), -rect.getRight(),
                                              -rect.getBottom(), -rect.getTop(),
                                              nearCulling, farCulling);
                break;
        }
    }

    void Camera::bindUniforms()
    {

    }
}
