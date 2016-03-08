//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_RECTANGLE_H
#define VOLKHVY_RECTANGLE_H

#include "glm/vec3.hpp"

namespace Gfx
{
	template <typename N>
	class Rectangle
	{
	public:
		auto GetWidth() -> N;
		auto GetHeight() -> N;

		auto GetTop() -> N;
		auto GetBottom() -> N;
		auto GetLeft() -> N;
		auto GetRight() -> N;

		auto GetCenter() -> glm::tvec2<N, glm::precision::defaultp>;
		auto GetMin() -> glm::tvec2<N, glm::precision::defaultp>;
		auto GetMax() -> glm::tvec2<N, glm::precision::defaultp>;
	};
}

#endif //VOLKHVY_RECTANGLE_H
