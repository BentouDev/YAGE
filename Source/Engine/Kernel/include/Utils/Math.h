//
// Created by Bentoo on 2017-04-02.
//

#ifndef YAGE_MATH_H
#define YAGE_MATH_H

namespace Math
{
	static constexpr float Pi = 3.14;
	static constexpr float Epsilon = 0.0000001f;

	constexpr inline float AngleToRadian(float angle)
	{
		return angle * (Pi / 180.0f);
	}
}

#endif //YAGE_MATH_H
