//
// Created by bentoo on 10/14/16.
//

#include "Core/Gfx/Shapes.h"

namespace Gfx
{
	namespace Shapes
	{
		namespace Cube
		{
			const uint8_t _triangles[] = {
					1, 0, 2,
					3, 2, 0,
					6, 4, 5,
					4, 6, 7,
					4, 7, 0,
					7, 3, 0,
					1, 2, 5,
					2, 6, 5,
					0, 1, 5,
					0, 5, 4,
					2, 3, 6,
					3, 7, 6,
			};

			const float _cube[] = {
					-0.5f,  0.5f,  0.5f,
					0.5f,  0.5f,  0.5f,
					0.5f, -0.5f,  0.5f,
					-0.5f, -0.5f,  0.5f,
					-0.5f,  0.5f, -0.5f,
					0.5f,  0.5f, -0.5f,
					0.5f, -0.5f, -0.5f,
					-0.5f, -0.5f, -0.5f,
			};

			const float _texcoords[] = {
					0.f, 0.f,
					0.f, 1.f,
					1.f, 1.f,
					0.f, 1.f,
					0.f, 1.f,
					1.f, 1.f,
					0.f, 1.f,
					0.f, 0.f
			};

			const float _normals[] = {
					-1.f,  1.f,  1.f,
					1.f,  1.f,  1.f,
					1.f, -1.f,  1.f,
					-1.f, -1.f,  1.f,
					-1.f,  1.f, -1.f,
					1.f,  1.f, -1.f,
					1.f, -1.f, -1.f,
					-1.f, -1.f, -1.f,
			};
		}
	}
}