//
// Created by bentoo on 7/8/16.
//

#ifndef GAME_SHADERMANAGER_H
#define GAME_SHADERMANAGER_H

#include <Container.h>
#include "Shader.h"

namespace Core
{
    class Engine;
}

namespace Resources
{
    class ShaderManager
    {
    protected:
        Utils::Container<Gfx::ShaderTrait> shaders;
        Utils::Container<Gfx::ShaderProgTrait> shaderProgs;
        Utils::Container<Gfx::ShaderResTrait> shaderRes;

    public:
        ShaderManager(Core::Engine& engine);
    };
}


#endif //GAME_SHADERMANAGER_H
