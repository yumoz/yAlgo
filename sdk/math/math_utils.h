#define _USE_MATH_DEFINES

#pragma once

#include "math_exports.h"
#include <cmath>

namespace yalgo {
namespace math {

// 角度和弧度转换函数
MATH_API double degreesToRadians(double degrees);
MATH_API double radiansToDegrees(double radians);

// 功率单位转换函数
MATH_API double wattsToKilowatts(double watts);
MATH_API double kilowattsToWatts(double kilowatts);
MATH_API double wattsToHorsepower(double watts);
MATH_API double horsepowerToWatts(double horsepower);

// 分贝功率转换函数
MATH_API double wattsToDbW(double watts);
MATH_API double dbWToWatts(double dbW);
MATH_API double wattsToDbm(double watts);
MATH_API double dbmToWatts(double dbm);
MATH_API double dbWToDbm(double dbW);
MATH_API double dbmToDbW(double dbm);

// 通用分贝转换函数
MATH_API double linearToDb(double linear, double reference = 1.0);
MATH_API double dbToLinear(double db, double reference = 1.0);

} // namespace math
} // namespace yalgo
