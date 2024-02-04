#pragma once
namespace application
{
	namespace math
	{
		constexpr float PAI = 3.141592f;

		static float RadianToAngle(const float rad)
		{
			return (rad * 180) / PAI;
		}
		static float AngleToRadian(const float ang)
		{
			return (ang * PAI) / 180;
		}
	}
}