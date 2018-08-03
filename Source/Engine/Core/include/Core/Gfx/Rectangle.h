//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_RECTANGLE_H
#define YAGE_RECTANGLE_H

#include <glm/vec2.hpp>

namespace Gfx
{
	template <typename N>
	class Rectangle
	{
		using vector2 = glm::tvec2<N, glm::precision::defaultp>;

		vector2 _min;
		vector2 _max;

	public:
		explicit Rectangle(N x, N y, N width, N height)
			: _min(x, y), _max(x + width, y + height)
		{ }

		explicit Rectangle(glm::vec2 min, glm::vec2 size)
			: _min(min), _max(min.x + size.x, min.y + size.y)
		{ }

		void setMinX(N value)	{ _min.x = value; }
		void setMinY(N value)	{ _min.y = value; }
		void setMaxX(N value)	{ _max.x = value; }
		void setMaxY(N value)	{ _max.y = value; }
		void setWidth(N value)	{ setMaxX(_min.x + value); }
		void setHeight(N value)	{ setMaxY(_min.y + value); }

		auto getWidth()			const noexcept -> N { return _max.x - _min.x; }
		auto getHeight()		const noexcept -> N { return _max.y - _min.y; }

		auto getSize()			const noexcept -> vector2 { return _max - _min; }

		auto getTop()			const noexcept -> N { return _max.y; }
		auto getBottom()		const noexcept -> N { return _min.y; }
		auto getLeft()			const noexcept -> N { return _min.x; }
		auto getRight()			const noexcept -> N { return _max.x; }

		auto getMin()			const noexcept -> vector2 { return _min; }
		auto getMax()			const noexcept -> vector2 { return _max; }

		auto getTopLeft()		const noexcept -> vector2 { return vector2(getLeft(),  getTop());    }
		auto getTopRight()		const noexcept -> vector2 { return vector2(getRight(), getTop());    }

		auto getBottomLeft()	const noexcept -> vector2 { return vector2(getLeft(),  getBottom()); }
		auto getBottomRight()	const noexcept -> vector2 { return vector2(getRight(), getBottom()); }

		auto getCenter()		const noexcept -> vector2
		{
			return glm::tvec2<N, glm::precision::defaultp> (
				getLeft() + getWidth() * 0.5, getBottom() + getHeight() * 0.5
			);
		}

		auto localToWorld(vector2 point) const noexcept -> vector2
		{
			auto size = getSize();
			auto dupa = point * size;
			auto kwas = dupa + _min;
			return kwas;
		}

		auto worldToLocal(vector2 point) const noexcept -> vector2
		{
			return point;
		}

		void toArray(N (&array)[8]) const noexcept
		{
			array[0] = _min.x;
			array[1] = _max.y,
			array[2] = _max.x;
			array[3] = _max.y;
			array[4] = _min.x;
			array[5] = _min.y;
			array[6] = _max.x;
			array[7] = _min.y;
		}
	};

	using rect_t  = Rectangle<float>;
	using recti_t = Rectangle<std::int32_t>;
}

#endif //YAGE_RECTANGLE_H
