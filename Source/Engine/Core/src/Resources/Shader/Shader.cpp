//
// Created by mrjaqbq on 21.05.16.
//

#include "Core/Resources/Shader/Shader.h"

namespace Gfx
{
	void ShaderProgram::init(GLuint handle)
	{
		_handle = handle;
	}

	void ShaderProgram::bind() const noexcept
	{
		if(_handle > 0)
			gl::UseProgram(_handle);
	}
}
