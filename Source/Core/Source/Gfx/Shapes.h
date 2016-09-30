//
// Created by mrjaqbq on 17.05.16.
//

#ifndef GAME_SHAPES_H
#define GAME_SHAPES_H

#include <cstdint>

namespace Gfx
{
	namespace Shapes
	{
		namespace Cube
		{
			uint8_t _triangles[] = {
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

			float _cube[] = {
					-0.5f,  0.5f,  0.5f,
					0.5f,  0.5f,  0.5f,
					0.5f, -0.5f,  0.5f,
					-0.5f, -0.5f,  0.5f,
					-0.5f,  0.5f, -0.5f,
					0.5f,  0.5f, -0.5f,
					0.5f, -0.5f, -0.5f,
					-0.5f, -0.5f, -0.5f,
			};

			float _normals[] = {
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


#endif //GAME_SHAPES_H
