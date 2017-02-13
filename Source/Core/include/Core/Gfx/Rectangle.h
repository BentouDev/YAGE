//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_RECTANGLE_H
#define YAGE_RECTANGLE_H

#include "glm/vec2.hpp"

namespace Gfx
{
	template <typename N>
	class Rectangle
	{
		glm::tvec2<N, glm::precision::defaultp> _min;
		glm::tvec2<N, glm::precision::defaultp> _max;

	public:
		explicit Rectangle(N x, N y, N width, N height)
			: _min(x, y), _max(x + width, y + height)
		{ }

		auto getWidth()		const noexcept -> N { return _max.x - _min.x; }
		auto getHeight()	const noexcept -> N { return _max.y - _min.y; }

		auto getTop()		const noexcept -> N { return _max.y; }
		auto getBottom()	const noexcept -> N { return _min.y; }
		auto getLeft()		const noexcept -> N { return _min.x; }
		auto getRight()		const noexcept -> N { return _max.x; }

		auto getMin()		const noexcept -> glm::tvec2<N, glm::precision::defaultp> { return _min; }
		auto getMax()		const noexcept -> glm::tvec2<N, glm::precision::defaultp> { return _max; }

		auto getCenter()	const noexcept -> glm::tvec2<N, glm::precision::defaultp>
		{
			return glm::tvec2<N, glm::precision::defaultp> (
				_min.x + getWidth() * 0.5, _min.y + getHeight() * 0.5
			);
		}
	};
}

#endif //YAGE_RECTANGLE_H
