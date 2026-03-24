/**
 * @file math_utils.h
 * @brief 数学工具函数
 * @author yAlgo Team
 * @date 2026-03-24
 */
#define _USE_MATH_DEFINES

#pragma once

#include "math_exports.h"
#include <cmath>

namespace yalgo {
namespace math {

/**
 * @brief 将角度转换为弧度
 * @param degrees 角度值
 * @return 弧度值
 */
MATH_API double degreesToRadians(double degrees);

/**
 * @brief 将弧度转换为角度
 * @param radians 弧度值
 * @return 角度值
 */
MATH_API double radiansToDegrees(double radians);

/**
 * @brief 将瓦特转换为千瓦
 * @param watts 瓦特值
 * @return 千瓦值
 */
MATH_API double wattsToKilowatts(double watts);

/**
 * @brief 将千瓦转换为瓦特
 * @param kilowatts 千瓦值
 * @return 瓦特值
 */
MATH_API double kilowattsToWatts(double kilowatts);

/**
 * @brief 将瓦特转换为马力
 * @param watts 瓦特值
 * @return 马力值
 */
MATH_API double wattsToHorsepower(double watts);

/**
 * @brief 将马力转换为瓦特
 * @param horsepower 马力值
 * @return 瓦特值
 */
MATH_API double horsepowerToWatts(double horsepower);

/**
 * @brief 将瓦特转换为 dBW
 * @param watts 瓦特值
 * @return dBW 值
 */
MATH_API double wattsToDbW(double watts);

/**
 * @brief 将 dBW 转换为瓦特
 * @param dbW dBW 值
 * @return 瓦特值
 */
MATH_API double dbWToWatts(double dbW);

/**
 * @brief 将瓦特转换为 dBm
 * @param watts 瓦特值
 * @return dBm 值
 */
MATH_API double wattsToDbm(double watts);

/**
 * @brief 将 dBm 转换为瓦特
 * @param dbm dBm 值
 * @return 瓦特值
 */
MATH_API double dbmToWatts(double dbm);

/**
 * @brief 将 dBW 转换为 dBm
 * @param dbW dBW 值
 * @return dBm 值
 */
MATH_API double dbWToDbm(double dbW);

/**
 * @brief 将 dBm 转换为 dBW
 * @param dbm dBm 值
 * @return dBW 值
 */
MATH_API double dbmToDbW(double dbm);

/**
 * @brief 将线性值转换为分贝值
 * @param linear 线性值
 * @param reference 参考值（默认为 1.0）
 * @return 分贝值
 */
MATH_API double linearToDb(double linear, double reference = 1.0);

/**
 * @brief 将分贝值转换为线性值
 * @param db 分贝值
 * @param reference 参考值（默认为 1.0）
 * @return 线性值
 */
MATH_API double dbToLinear(double db, double reference = 1.0);

} // namespace math
} // namespace yalgo
