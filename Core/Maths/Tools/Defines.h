#ifndef _DEFINES_H_
#define _DEFINES_H_

namespace engine
{
	namespace core
	{
		namespace maths
		{
#ifndef PI
#define PI 3.14159265358979323846264338327950288
#endif

#ifndef PI_f
#define PI_f 3.14159265358979323846264338327950288f
#endif

#ifndef INV_PI
#define INV_PI 0.318309886183790702318674448179081082
#endif

#ifndef INV_PI_f
#define INV_PI_f 0.318309886183790702318674448179081082f
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.2957795130823228646477218717336655
#endif

#ifndef RAD_TO_DEG_f
#define RAD_TO_DEG_f 57.2957795130823228646477218717336655f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943297520512714982032776
#endif

#ifndef DEG_TO_RAD_f
#define DEG_TO_RAD_f 0.017453292519943297520512714982032776f
#endif

#ifndef MIN
#define MIN(leftVal, rightVal) \
		((leftVal) < (rightVal) ? (leftVal) : (rightVal))
#endif

#ifndef MAX
#define MAX(leftVal, rightVal) \
		((leftVal) > (rightVal) ? (leftVal) : (rightVal))
#endif

#ifndef CUSTOM_MIN
#define CUSTOM_MIN(leftVal, rightVal, leftReturn, rightReturn) \
		((leftVal) < (rightVal) ? (leftReturn) : (rightReturn))
#endif

#ifndef CUSTOM_MAX
#define CUSTOM_MAX(leftVal, rightVal, leftReturn, rightReturn) \
		((leftVal) > (rightVal) ? (leftReturn) : (rightReturn))
#endif

#ifndef CLAMP
#define CLAMP(value, minValue, maxValue) \
		(MAX((minValue), MIN((value), (maxValue))))
#endif

#ifndef LERP
#define LERP(leftVal, rightVal, ratio) \
		((leftVal) + (((rightVal) - (leftVal)) * (ratio)))
#endif
			// -180 < angle < 180
			inline float NormalizeAngle(float angle)
			{
				angle = fmodf(angle + 180, 360);
				if (angle < 0)
					angle += 360;

				return angle - 180;
			}
		}
	}
}

#endif