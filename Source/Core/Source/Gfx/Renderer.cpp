//
// Created by MrJaqbq on 2016-03-26.
//

#include <cstdint>
#include "Renderer.h"

namespace Gfx
{
	// this is how it may look for opengl
	// Vulkan will have different loop for sure
	// So it should be different base on template or something
	void Renderer::Draw()
	{
		/*for(uint16_t rc = 0; rc < RenderCommands; rc++)
		{
			auto command = RenderCommands[rc];
			for(uint16_t p = 0; p < command.passesCount; p++)
			{
				auto pass = command.passes[p];
				bindRenderTarget(pass.renderTarget);

				if(pass.isClear)
					clearBuffer(pass.renderTarget);

				for(uint16_t sh = 0; sh < pass.shaderCount; sh++)
				{
					auto shader = pass.shaders[sh];
					bindShaderProgram(shader.program);
					bindShaderData(shader.data);
					for(uint16_t m = 0; m < shader.materialCount; m++)
					{
						auto material = shader.materials[m];
						bindMaterialData(material.data);

						for(uint16_t sm = 0; sm < material.subMeshesCount; sm++)
						{
							drawCall(sm);
						}
					}
				}

				if(pass.isBuffered)
					swapBuffers(pass.renderTarget);
			}
		}*/
	}
}
