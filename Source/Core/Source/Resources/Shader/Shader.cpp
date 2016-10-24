//
// Created by mrjaqbq on 21.05.16.
//

#include "Shader.h"

namespace Gfx
{
	void ShaderProgram::init(GLint handle)
	{
		if(_handle != -1)
			_handle = handle;
	}

	void ShaderProgram::cleanUp() noexcept
	{
		// TODO: nicier api, maybe generic handle overload?
		if(_handle != -1)
		{
			gl::DeleteProgram(_handle);
			_handle = -1;
		}
	}

	void ShaderProgram::bind() const noexcept
	{
		if(_handle != -1)
			gl::UseProgram(_handle);
	}
}
