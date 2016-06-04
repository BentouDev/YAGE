//
// Created by mrjaqbq on 04.06.16.
//

#include "Generated/gl_core_3_3.hpp"
#include "OpenGLCommands.h"
#include "../CommandQueue.h"

namespace Gfx
{
	namespace Commands
	{
		void draw(Command& cmd)
		{
			GLenum mode = gl::TRIANGLES;
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, cmd.index);
			gl::DrawElements(mode, cmd.count, gl::UNSIGNED_BYTE, (void*)0);
		}
	}
}